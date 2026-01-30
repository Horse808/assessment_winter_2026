# 作业1：协议帧解析（C++工程化 + 单测）

## 背景故事
你刚加入 RoboMaster 战队软件组，负责给「云台主控」和「裁判系统/上位机」之间加一条稳定的串口通信链路。
实测发现：数据流里会夹杂噪声字节、偶发丢包、以及“半包”（一次只收到一部分数据）。
你的任务是实现一个**流式**协议帧解析器：给你一段连续字节流，你要从中正确恢复出完整帧。

---
## 协议定义（本次作业用的简化版）
线上的一帧数据格式如下（小端序）：

| 字段 | 长度 | 说明 |
|---|---:|---|
| SOF | 2 | 固定 `0xA5 0x5A` |
| version | 1 | 协议版本，本作业固定用 1 |
| payload_len | 2 | payload 字节数 |
| seq | 2 | 包序号（回包/丢包定位用） |
| type | 1 | 消息类型 |
| payload | N | 数据负载 |
| crc16 | 2 | CRC16-CCITT，覆盖 `version..payload` |

CRC16-CCITT 参数：poly=0x1021，init=0xFFFF。

---
## 任务A（工程题）：实现 rmproto 库
你需要完成 `include/rmproto/frame.hpp` 中的接口实现（已经在 `src/frame.cpp` 给了参考实现；你可以删掉并自己实现，或基于它改进，但最终以测试通过为准）。

### 必须实现
- `rmproto::Encode(const Frame&)`：把结构体编码成字节数组
- `rmproto::TryDecode(std::vector<uint8_t>& buffer, Frame& out)`：从**可变缓冲区**中尝试解析一帧
  - 成功：返回 true，并从 buffer 中移除已消费的字节
  - 数据不足：返回 false，buffer 不变
  - 数据错误（CRC错/长度不合理等）：丢弃部分字节并继续尝试重同步（不能死循环）

### 关键点
- 要能处理：前导垃圾字节、粘包、多帧连在一起、半包。
- 不要一次性假设 buffer 开头就是 SOF；需要搜索 SOF 并同步。

---
## 任务B（算法题，可用 Python 对照）：CRC16
实现/理解 CRC16-CCITT。
- C++：在 `Crc16Ccitt` 中实现
- Python：可用 `tools/ref.py` 做对照验证（不是必须提交）

---
## 如何构建与运行
在仓库根目录：

```powershell
cmake -S assignment_1 -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

### CLI 试玩
```powershell
# 编码：seq=42 type=7 payload=01 02 A0 FF
.\build\Release\rmproto_cli.exe encode 42 7 "01 02 A0 FF"

# 解码：把上一步输出粘贴进来
.\build\Release\rmproto_cli.exe decode "A5 5A ..."
```

---
## 提交要求
- 代码必须通过 CI（.github/workflows/ci.yml）
- `assignment_1` 下 `clang-format` 规范（建议提交前对 C++ 文件格式化）
- 建议：小步提交（commit message 清晰），不要一次性大坨
