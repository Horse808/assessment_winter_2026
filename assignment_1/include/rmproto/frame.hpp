#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace rmproto {

struct Frame {
  uint8_t version = 1;
  uint16_t seq = 0;
  uint8_t type = 0;
  std::vector<uint8_t> payload;
};

// CRC16-CCITT (poly 0x1021, init 0xFFFF)
uint16_t Crc16Ccitt(const uint8_t* data, size_t len);

// Serialize a frame into bytes.
// Wire format (little-endian fields):
//   SOF[2] = 0xA5 0x5A
//   version[1]
//   payload_len[2]  (number of payload bytes)
//   seq[2]
//   type[1]
//   payload[payload_len]
//   crc16[2]  (covers version..payload)
std::vector<uint8_t> Encode(const Frame& f);

// Try to decode one frame from a byte stream.
// - If a valid frame is found, sets out and returns true, and removes consumed bytes from buffer.
// - If not enough data, returns false and keeps buffer as-is.
// - If data is invalid, discards bytes until a possible SOF and continues.
bool TryDecode(std::vector<uint8_t>& buffer, Frame& out);

// Helpers for CLI: parse hex string like "A5 5A 01 00" or "a55a0100".
bool ParseHexBytes(const std::string& text, std::vector<uint8_t>& out);
std::string ToHex(const std::vector<uint8_t>& bytes);

}  // namespace rmproto
