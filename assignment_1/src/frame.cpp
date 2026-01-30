#include "rmproto/frame.hpp"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>

namespace rmproto {

namespace {
constexpr uint8_t kSof0 = 0xA5;
constexpr uint8_t kSof1 = 0x5A;

void AppendLe16(std::vector<uint8_t>& out, uint16_t v) {
  out.push_back(static_cast<uint8_t>(v & 0xFF));
  out.push_back(static_cast<uint8_t>((v >> 8) & 0xFF));
}

uint16_t ReadLe16(const std::vector<uint8_t>& in, size_t offset) {
  return static_cast<uint16_t>(in[offset]) |
         (static_cast<uint16_t>(in[offset + 1]) << 8);
}

bool IsHexChar(char c) {
  return std::isxdigit(static_cast<unsigned char>(c)) != 0;
}

int HexVal(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
  if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
  return -1;
}

}  // namespace

uint16_t Crc16Ccitt(const uint8_t* data, size_t len) {
  // TODO(student): Implement CRC16-CCITT.
  // Parameters: poly=0x1021, init=0xFFFF.
  // This CRC must cover the bytes: version..payload (i.e., everything after SOF and
  // before crc16). Tests depend on this.
  (void)data;
  (void)len;
  return 0;
}

std::vector<uint8_t> Encode(const Frame& f) {
  // TODO(student): Serialize a Frame into the wire format.
  // Wire format (little-endian fields):
  //   SOF[2] = 0xA5 0x5A
  //   version[1]
  //   payload_len[2]
  //   seq[2]
  //   type[1]
  //   payload[payload_len]
  //   crc16[2] (CRC16-CCITT over version..payload)
  //
  // This placeholder intentionally produces an incorrect frame so unit tests fail,
  // while still allowing the project (and CLI) to compile.
  std::vector<uint8_t> out;
  out.reserve(2 + 1 + 2 + 2 + 1 + f.payload.size() + 2);

  out.push_back(kSof0);
  out.push_back(kSof1);
  out.push_back(f.version);
  AppendLe16(out, static_cast<uint16_t>(f.payload.size()));
  AppendLe16(out, f.seq);
  out.push_back(f.type);
  out.insert(out.end(), f.payload.begin(), f.payload.end());

  // TODO(student): replace with real CRC.
  AppendLe16(out, 0);
  return out;
}

bool TryDecode(std::vector<uint8_t>& buffer, Frame& out) {
  // TODO(student): Stream parser.
  // Requirements (see README + tests):
  // - buffer is a stream that may start with garbage.
  // - must search for SOF (0xA5 0x5A) to resync.
  // - if not enough bytes for a full frame: return false and keep buffer unchanged.
  // - if a candidate frame has bad CRC / invalid length: discard some bytes and
  //   continue searching (must not dead-loop).
  // - on success: fill `out`, erase consumed bytes from `buffer`, return true.
  (void)buffer;
  (void)out;
  return false;
}

bool ParseHexBytes(const std::string& text, std::vector<uint8_t>& out) {
  // Accept:
  // - "A5 5A 01 00" (spaces)
  // - "A5,5A,01,00" (commas)
  // - "0xA5 0x5A" (optional 0x prefix per byte)
  // - "a55a0100" (no separators)
  out.clear();

  size_t i = 0;
  auto skip_sep = [&](void) {
    while (i < text.size()) {
      char c = text[i];
      if (std::isspace(static_cast<unsigned char>(c)) || c == ',') {
        ++i;
        continue;
      }
      break;
    }
  };

  while (true) {
    skip_sep();
    if (i >= text.size()) break;

    // Optional 0x prefix (only when it appears exactly as "0x"/"0X").
    if (text[i] == '0' && (i + 1) < text.size() && (text[i + 1] == 'x' || text[i + 1] == 'X')) {
      i += 2;
    }

    if ((i + 1) >= text.size()) return false;
    if (!IsHexChar(text[i]) || !IsHexChar(text[i + 1])) return false;

    int hi = HexVal(text[i]);
    int lo = HexVal(text[i + 1]);
    if (hi < 0 || lo < 0) return false;

    out.push_back(static_cast<uint8_t>((hi << 4) | lo));
    i += 2;
  }

  return true;
}

std::string ToHex(const std::vector<uint8_t>& bytes) {
  std::ostringstream oss;
  oss << std::hex << std::setfill('0');
  for (size_t i = 0; i < bytes.size(); ++i) {
    if (i) oss << ' ';
    oss << std::setw(2) << static_cast<int>(bytes[i]);
  }
  return oss.str();
}

}  // namespace rmproto
