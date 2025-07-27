## 🔐 GIỚI THIỆU DỰ ÁN SMART DOOR LOCK

---

### 1. Bối cảnh và lý do chọn đề tài

Trong những năm gần đây, cùng với tốc độ đô thị hóa và sự phát triển nhanh chóng của công nghệ IoT, nhu cầu đảm bảo an ninh cá nhân và tài sản ngày càng trở nên cấp thiết. Các phương pháp khóa cửa truyền thống—dù đã tồn tại hàng trăm năm—đang dần bộc lộ nhiều nhược điểm trong thế giới hiện đại. Cụ thể:

* 🔓 **Chìa khóa có thể bị sao chép dễ dàng**, không kiểm soát được ai đang giữ bản sao;
* 🛠️ **Dễ bị cạy phá hoặc phá bằng vũ lực**;
* 🧍‍♂️ **Không hỗ trợ các nhu cầu hiện đại** như cấp quyền mở cửa tạm thời cho người thân, bạn bè, hoặc nhân viên giao hàng;
* 🔐 **Không có chức năng giám sát trạng thái hoặc lịch sử mở khóa**.

Trước những hạn chế đó, khóa thông minh (**Smart Lock**) được xem là giải pháp thay thế tiên tiến. Không chỉ đơn thuần thay đổi phương pháp mở cửa, các hệ thống này tích hợp nhiều công nghệ như xác thực số, kết nối không dây, lưu trữ thông tin người dùng và tương tác theo thời gian thực. Dự án "Smart Door Lock" được xây dựng nhằm hiện thực hóa giải pháp khóa thông minh bằng cách ứng dụng các thiết bị phần cứng chi phí thấp nhưng mạnh mẽ như Arduino Uno, cảm biến siêu âm, RFID, Bluetooth, v.v.

---

### 2. Mục tiêu của dự án

Dự án hướng đến thiết kế một hệ thống **khóa cửa thông minh**, có khả năng:

* ✅ **Xác thực đa phương thức** (mã PIN, thẻ RFID, điện thoại Bluetooth) để nâng cao bảo mật;
* 📲 **Điều khiển từ xa và cập nhật trạng thái** qua ứng dụng điện thoại;
* 🔋 **Tiết kiệm năng lượng**, chỉ kích hoạt khi có người đến gần;
* 🧩 **Thiết kế mô-đun, dễ mở rộng**, tích hợp thêm các công nghệ mới trong tương lai như vân tay, camera, cloud;
* 📊 **Theo dõi trạng thái và lịch sử truy cập**, hỗ trợ quản lý hệ thống hiệu quả hơn.

---

### 3. Thành phần phần cứng chính

Hệ thống được xây dựng trên nền **Arduino Uno Rev3**, đóng vai trò là bộ xử lý trung tâm, điều khiển toàn bộ hoạt động của hệ thống. Các thành phần phần cứng chính bao gồm:

* 🧠 **Arduino Uno**: Nhận tín hiệu từ bàn phím, RFID, cảm biến và xử lý xác thực.
* 👣 **HC-SR04 (Cảm biến siêu âm)**: Phát hiện người đến gần (trong khoảng < 50 cm), giúp hệ thống tự động “thức dậy” từ chế độ chờ.
* 🔢 **Keypad 4x4**: Nhập mã PIN cá nhân với độ dài 6 chữ số.
* 📶 **RC522 RFID**: Đọc UID của thẻ từ không tiếp xúc, hoạt động ở tần số 13.56 MHz.
* 📲 **HC-05 Bluetooth**: Kết nối điện thoại với hệ thống, cho phép gửi lệnh như “OPEN”, “SETPASS”.
* 🔁 **Relay module**: Đóng/ngắt dòng điện cấp cho khóa từ, đóng vai trò trung gian giữa vi điều khiển và thiết bị công suất.
* 🧲 **Khóa từ (Magnetic Lock)**: Giữ cửa ở trạng thái khóa bằng lực điện từ. Tự động mở trong 5 giây khi xác thực thành công.
* 💾 **EEPROM**: Lưu trữ mã PIN và danh sách UID thẻ RFID ngay cả khi mất điện.
* 🖥️ **LCD I2C 16x2**: Hiển thị thông báo như “Welcome”, “Access Granted”, “Wrong Password”.
* 🌐 **NodeMCU ESP8266 (tùy chọn)**: Kết nối Wi-Fi, gửi dữ liệu truy cập lên nền tảng IoT (ThingSpeak).

---

### 4. Cấu trúc và nguyên lý hoạt động

Hệ thống được tổ chức thành 4 khối chức năng chính:

#### 🔹 4.1. Khối đầu vào

Bao gồm:

* **Cảm biến siêu âm**: Giám sát khoảng cách từ cửa đến người dùng, chỉ khi có người đến gần thì hệ thống mới “thức dậy”.
* **Keypad**: Nhận mã PIN từ người dùng.
* **RFID**: Nhận UID từ thẻ hoặc móc khóa.
* **Bluetooth**: Nhận lệnh điều khiển từ điện thoại.

#### 🔹 4.2. Khối xử lý

* **Arduino Uno** là trung tâm, tiếp nhận dữ liệu từ các thiết bị đầu vào và đối chiếu với dữ liệu lưu trữ trong **EEPROM**.
* Nếu dữ liệu hợp lệ → Gửi lệnh bật **relay**.
* Nếu không hợp lệ → Hiển thị thông báo lỗi trên **LCD** và không thực hiện mở khóa.

#### 🔹 4.3. Khối đầu ra

* **Relay** được kích hoạt trong thời gian ngắn (\~5 giây) để cấp điện cho **khóa từ**.
* Sau khi hết thời gian, relay tự động tắt và cửa tự khóa lại.
* **LCD** hiển thị trạng thái (mở/khóa, đúng/sai mã, v.v.).

#### 🔹 4.4. Khối giao tiếp

* Kết nối Bluetooth với điện thoại để gửi lệnh điều khiển hoặc thay đổi cấu hình (mã PIN, thẻ mới).
* Tùy chọn **ESP8266 Wi-Fi** để truyền thông tin lên **cloud** giúp người dùng giám sát từ xa.

---

### 5. Các phương thức xác thực hỗ trợ

Hệ thống hỗ trợ xác thực đa phương thức để tăng mức độ bảo mật và tiện dụng:

* 🔐 **Mã PIN (Keypad)**: Mỗi người dùng có thể nhập mã bí mật cá nhân. Hệ thống có khả năng ghi nhớ và so sánh qua EEPROM.
* 🪪 **Thẻ từ RFID**: Mỗi thẻ mang UID riêng biệt. Chỉ những UID đã được cấp quyền mới được mở khóa.
* 📱 **Bluetooth (Điện thoại)**: Người dùng có thể mở khóa thông qua ứng dụng gửi lệnh “OPEN” hoặc “SET PASSWORD”.
* 👁 **Cảm biến siêu âm**: Kích hoạt các thành phần khác chỉ khi phát hiện có người → tiết kiệm năng lượng và tăng bảo mật.

---

### 6. Ưu điểm nổi bật của hệ thống

* 🔐 **An toàn cao**: Dữ liệu được kiểm tra qua nhiều lớp xác thực; có thể mở rộng lên xác thực kép (PIN + RFID).
* ⚡ **Tiết kiệm năng lượng**: Không tiêu thụ điện liên tục nhờ chế độ chờ chủ động.
* 📲 **Điều khiển từ xa**: Người dùng có thể mở khóa không cần tiếp xúc, phù hợp cho tình huống khẩn cấp.
* 🧩 **Dễ mở rộng**: Kiến trúc mô-đun giúp tích hợp dễ dàng các tính năng như vân tay, camera, IoT cloud.
* 🛠 **Dễ bảo trì và lập trình**: Sử dụng thư viện mã nguồn mở và giao diện lập trình thân thiện (Arduino IDE).
* 📶 **Khả năng kết nối mạng**: Có thể giám sát và cập nhật thông tin từ xa thông qua Wi-Fi và nền tảng IoT.

---

### 7. Khả năng mở rộng trong tương lai

Hệ thống được thiết kế để có thể nâng cấp linh hoạt với các module mở rộng như:

* 📷 **Camera nhận diện khuôn mặt**.
* 🧬 **Cảm biến vân tay** để tăng bảo mật sinh trắc học.
* ☁️ **Ghi log và thống kê truy cập** trên Firebase hoặc Google Sheets.
* 📡 **GSM Module** để gửi tin nhắn SMS cảnh báo khi có truy cập lạ.
* 🔒 **Tích hợp với hệ thống nhà thông minh (Smart Home)** như Alexa, Google Home...

---

### 8. Ứng dụng thực tế

Hệ thống **Smart Door Lock** phù hợp với nhiều môi trường khác nhau:

* 🏠 **Nhà riêng hoặc căn hộ**: đảm bảo an toàn khi vắng nhà, dễ chia sẻ quyền truy cập cho người thân.
* 🏢 **Văn phòng công ty nhỏ**: kiểm soát quyền ra vào, thay thế khóa chìa truyền thống.
* 🧪 **Phòng lab/phòng thiết bị**: giới hạn quyền truy cập với người dùng được phân quyền.
* 🏬 **Kho hàng nhỏ**: tích hợp kiểm soát ra vào và log lịch sử để truy xuất khi cần thiết.

---
