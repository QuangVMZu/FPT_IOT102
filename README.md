## 🔐 GIỚI THIỆU DỰ ÁN SMART DOOR LOCK

### 1. Bối cảnh và lý do chọn đề tài

Trong kỷ nguyên của đô thị hóa nhanh và sự phát triển mạnh mẽ của công nghệ số, nhu cầu về các giải pháp an ninh hiện đại – đặc biệt là trong lĩnh vực kiểm soát truy cập – ngày càng gia tăng. Hệ thống khóa cơ truyền thống ngày nay đang bộc lộ nhiều điểm yếu như:

* Dễ bị sao chép chìa khóa,
* Dễ bị phá khóa thủ công (lock picking, phá bằng lực),
* Không hỗ trợ kiểm soát truy cập linh hoạt (ví dụ: không thể chia sẻ quyền mở cửa tạm thời).

Với sự phổ biến của **Internet of Things (IoT)**, các hệ thống **Smart Lock** ra đời nhằm thay thế khóa truyền thống bằng các phương pháp xác thực điện tử, cho phép điều khiển và giám sát từ xa thông qua các thiết bị thông minh. Xuất phát từ thực tiễn đó, nhóm thực hiện đề tài "Smart Door Lock" – một hệ thống khóa thông minh tích hợp đa phương thức xác thực và có thể mở rộng thành một phần của hệ sinh thái nhà thông minh.

---

### 2. Mục tiêu của dự án

Dự án hướng đến việc thiết kế và triển khai **một hệ thống khóa cửa thông minh**, tích hợp các công nghệ điện tử và truyền thông hiện đại, với các mục tiêu cụ thể:

* ✅ **Tăng cường bảo mật**: sử dụng xác thực đa lớp (mã PIN, RFID, Bluetooth).
* ✅ **Tối ưu trải nghiệm người dùng**: cung cấp phản hồi thời gian thực, dễ sử dụng ngay cả với người không chuyên.
* ✅ **Hỗ trợ giám sát và điều khiển từ xa**: thông qua Bluetooth hoặc kết nối Wi-Fi với nền tảng IoT (ThingSpeak).
* ✅ **Tiết kiệm năng lượng và dễ bảo trì**: kích hoạt theo sự hiện diện và thiết kế mô-đun linh kiện.

---

### 3. Thành phần phần cứng chính

| Thành phần                    | Chức năng                                                              |
| ----------------------------- | ---------------------------------------------------------------------- |
| **Arduino Uno**               | Bộ xử lý trung tâm, xử lý tín hiệu vào/ra và thực hiện logic xác thực. |
| **HC-SR04**                   | Cảm biến siêu âm phát hiện người đến gần, kích hoạt hệ thống.          |
| **Keypad 4x4**                | Nhập mã PIN để xác thực.                                               |
| **RC522 RFID**                | Quét thẻ từ chuẩn MIFARE để nhận dạng người dùng.                      |
| **HC-05**                     | Mô-đun Bluetooth, điều khiển hệ thống từ điện thoại.                   |
| **Relay Module**              | Kích hoạt hoặc ngắt dòng điện đến **khóa từ** sau khi xác thực.        |
| **Khóa từ**                   | Cố định cửa bằng lực điện từ, tự động mở khi có tín hiệu hợp lệ.       |
| **NodeMCU ESP8266** (mở rộng) | Kết nối Wi-Fi, gửi dữ liệu truy cập lên nền tảng đám mây ThingSpeak.   |
| **LCD I2C 16x2**              | Hiển thị trạng thái hệ thống, hướng dẫn người dùng.                    |
| **EEPROM**                    | Lưu trữ mã PIN và UID thẻ RFID.                                        |

---

### 4. Cấu trúc hoạt động

Hệ thống được chia thành 4 khối chính:

* 🔹 **Khối đầu vào**: bao gồm cảm biến siêu âm, bàn phím và đầu đọc RFID. Cảm biến sẽ kích hoạt hệ thống khi phát hiện người dùng đến gần. Sau đó, người dùng có thể xác thực bằng mã PIN hoặc quét thẻ từ.

* 🔹 **Khối xử lý**: Arduino sẽ so sánh dữ liệu đầu vào với các mã được lưu trong EEPROM. Nếu trùng khớp, sẽ thực hiện mở khóa.

* 🔹 **Khối đầu ra**: bao gồm relay và khóa từ. Khi xác thực thành công, relay sẽ dẫn dòng cho khóa từ hoạt động trong một khoảng thời gian ngắn (\~5s), sau đó tự động khóa lại.

* 🔹 **Khối giao tiếp**: HC-05 hỗ trợ điều khiển qua điện thoại bằng lệnh như “OPEN”, “ADDRFID”. NodeMCU ESP8266 giúp gửi dữ liệu truy cập lên cloud ThingSpeak (mở rộng).

---

### 5. Các phương thức xác thực được hỗ trợ

* 🔐 **Mã PIN**: nhập qua bàn phím 4x4, độ dài tối đa 6 chữ số.
* 🔐 **Thẻ RFID**: chuẩn MIFARE, UID được so sánh với danh sách trong EEPROM.
* 📲 **Bluetooth**: gửi lệnh từ điện thoại như "OPEN" để mở khóa.
* 👁 **Phát hiện người đến gần**: cảm biến siêu âm giúp bật hệ thống khi có người lại gần, tiết kiệm năng lượng.

---

### 6. Ưu điểm nổi bật của hệ thống

* 💡 **Xác thực đa lớp (Multi-Factor)**: tăng mức độ bảo mật.
* 🧩 **Thiết kế mô-đun**: dễ bảo trì, dễ mở rộng với cảm biến vân tay, camera, Wi-Fi...
* 🔋 **Tiết kiệm năng lượng**: các thành phần như Bluetooth và màn hình chỉ kích hoạt khi cần thiết.
* 📶 **Giám sát thời gian thực**: có thể gửi log truy cập lên cloud (ThingSpeak).
* 📱 **Giao diện người dùng thân thiện**: hiển thị hướng dẫn và trạng thái rõ ràng.

---

### 7. Khả năng mở rộng trong tương lai

Hệ thống có kiến trúc mở, cho phép tích hợp thêm các tính năng cao cấp như:

* 📷 Nhận diện khuôn mặt bằng camera module.
* 🔑 Cảm biến vân tay cho xác thực sinh trắc học.
* ☁️ Lưu trữ nhật ký truy cập trên Firebase, Blynk hoặc nền tảng IoT khác.
* 📲 Tích hợp app mobile với giao diện người dùng.

---

### 8. Tính ứng dụng thực tiễn

Hệ thống phù hợp với các môi trường sau:

* 🏠 Nhà ở cá nhân, chung cư.
* 🏢 Văn phòng làm việc, công ty nhỏ.
* 🧪 Phòng lab, phòng thiết bị.
* 🚪 Nhà kho, phòng lưu trữ cần kiểm soát truy cập.

---
