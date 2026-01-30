"""Reference implementation for quick validation (optional).

Usage:
  python tools/ref.py encode <seq> <type> <payload_hex>
  python tools/ref.py decode <stream_hex>

Hex formats accepted: "a55a..." or "A5 5A ...".
"""

import sys

SOF = bytes([0xA5, 0x5A])


def crc16_ccitt(data: bytes) -> int:
    crc = 0xFFFF
    for b in data:
        crc ^= b << 8
        for _ in range(8):
            if crc & 0x8000:
                crc = ((crc << 1) ^ 0x1021) & 0xFFFF
            else:
                crc = (crc << 1) & 0xFFFF
    return crc


def parse_hex(s: str) -> bytes:
    s = s.replace("0x", "").replace("0X", "")
    s = "".join(ch for ch in s if ch.strip() and ch != ",")
    if len(s) % 2:
        raise ValueError("odd hex length")
    return bytes.fromhex(s)


def to_hex(b: bytes) -> str:
    return " ".join(f"{x:02x}" for x in b)


def encode(seq: int, typ: int, payload: bytes) -> bytes:
    version = 1
    header = bytes([version]) + len(payload).to_bytes(2, "little") + seq.to_bytes(2, "little") + bytes([typ])
    body = header + payload
    crc = crc16_ccitt(body).to_bytes(2, "little")
    return SOF + body + crc


def try_decode(stream: bytes):
    i = stream.find(SOF)
    if i < 0 or len(stream) - i < 10:
        return None
    s = stream[i:]
    version = s[2]
    payload_len = int.from_bytes(s[3:5], "little")
    total = 2 + 1 + 2 + 2 + 1 + payload_len + 2
    if len(s) < total:
        return None
    body = s[2: total - 2]
    got = int.from_bytes(s[total - 2: total], "little")
    exp = crc16_ccitt(body)
    if got != exp:
        return None
    seq = int.from_bytes(s[5:7], "little")
    typ = s[7]
    payload = s[8: 8 + payload_len]
    return version, seq, typ, payload


def main(argv: list[str]) -> int:
    if len(argv) < 2:
        print("usage: ref.py encode|decode ...", file=sys.stderr)
        return 2
    if argv[1] == "encode" and len(argv) == 5:
        seq = int(argv[2])
        typ = int(argv[3])
        payload = parse_hex(argv[4])
        print(to_hex(encode(seq, typ, payload)))
        return 0
    if argv[1] == "decode" and len(argv) == 3:
        stream = parse_hex(argv[2])
        r = try_decode(stream)
        if not r:
            print("no valid frame", file=sys.stderr)
            return 1
        version, seq, typ, payload = r
        print(f"version={version} seq={seq} type={typ} payload={to_hex(payload)}")
        return 0
    print("bad args", file=sys.stderr)
    return 2


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
