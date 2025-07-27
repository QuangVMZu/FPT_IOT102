## 🔐 **Giới thiệu Hệ thống Smart Door Lock**

### 1. Mục tiêu hệ thống

Hệ thống **Smart Door Lock** được phát triển nhằm tăng cường mức độ bảo mật và tiện ích cho người dùng trong việc quản lý quyền ra vào tại nhà riêng, văn phòng hoặc các khu vực hạn chế truy cập. Khác với các loại khóa truyền thống dễ bị sao chép chìa hoặc bị phá, hệ thống này ứng dụng công nghệ điều khiển tự động và xác thực điện tử để đảm bảo chỉ những người được ủy quyền mới có thể mở cửa. Đồng thời, hệ thống hỗ trợ điều khiển từ xa và phản hồi theo thời gian thực, phù hợp với xu hướng **Internet of Things (IoT)**.

---

### 2. Nền tảng phần cứng

Hệ thống được xây dựng dựa trên nền tảng **Arduino Uno R3**, đóng vai trò là bộ điều khiển trung tâm, điều phối hoạt động của các thiết bị ngoại vi thông qua các chân digital, analog, và giao tiếp I2C/SPI. Các thành phần phần cứng chính bao gồm:

* 📶 **RFID RC522**: Là module đọc thẻ từ hoạt động ở tần số 13.56 MHz. Thiết bị cho phép xác thực người dùng bằng cách quét thẻ RFID được cấp sẵn. Việc sử dụng giao tiếp SPI với Arduino giúp truyền dữ liệu nhanh và ổn định.

* 🔢 **Keypad 4x4 Matrix**: Dùng để nhập mã PIN thay thế hoặc bổ sung cho thẻ RFID. Mỗi phím được đọc qua ma trận hàng-cột, cho phép tiết kiệm chân I/O.

* 📲 **HC-06 Bluetooth Module**: Cho phép người dùng điều khiển hệ thống qua ứng dụng Android. Module này hỗ trợ giao tiếp UART (9600 baud mặc định), giúp truyền lệnh mở khóa hoặc kiểm tra trạng thái từ xa.

* 🧠 **HC-SR04**: Cảm biến siêu âm giúp phát hiện sự hiện diện trước cửa bằng cách đo khoảng cách. Khi có người đến gần trong phạm vi cấu hình (ví dụ: < 80 cm), hệ thống sẽ tự động kích hoạt màn hình và các chế độ chờ nhập mã hoặc quét thẻ.

* 📺 **OLED SSD1306 0.96”**: Hiển thị trạng thái hệ thống (chào mừng, đúng mã, sai mã, mở khóa thành công...) và hướng dẫn người dùng thao tác.

* 🔧 **Servo SG90**: Điều khiển cơ cấu khóa cửa vật lý (như lẫy chốt) bằng cách xoay một góc nhất định khi có lệnh mở khóa.

* 💡 **LED đỏ/xanh**: Cảnh báo trạng thái truy cập – xanh cho phép, đỏ từ chối.

* ⚡ **Relay module**: Dùng để điều khiển các thiết bị điện bổ trợ (nếu có), ví dụ đèn chiếu sáng hoặc còi báo động.

* 🔋 **Nguồn điện**: Hệ thống sử dụng 3 viên pin **18650 3.7V** mắc nối tiếp, cho tổng điện áp khoảng 12.6V. Một mạch giảm áp Buck Converter (ví dụ: LM2596) được dùng để hạ áp xuống 5V và 3.3V tùy theo yêu cầu của từng module (ví dụ RFID cần 3.3V, servo cần 5V).

---

### 3. Tính năng chính

* ✅ **Xác thực đa phương thức**: Người dùng có thể lựa chọn sử dụng thẻ từ RFID, mã PIN hoặc ứng dụng Bluetooth để mở khóa. Tất cả phương thức đều được xử lý bảo mật, tránh khả năng truy cập trái phép.

* 🔐 **Cơ chế điều khiển khóa thông minh**: Khi xác thực thành công, động cơ servo SG90 sẽ quay một góc cố định (thường 90°) để kéo chốt cửa, sau đó tự động khóa lại sau thời gian nhất định.

* 🧠 **Tự kích hoạt khi phát hiện người**: Nhờ cảm biến siêu âm HC-SR04, hệ thống có thể tiết kiệm năng lượng bằng cách chỉ bật màn hình và đèn LED khi có người lại gần.

* 🖥️ **Giao tiếp người dùng rõ ràng**: Tất cả thông tin phản hồi đều hiển thị trên màn hình OLED và đèn LED, giúp người dùng dễ dàng nhận biết tình trạng hệ thống.

* 📲 **Điều khiển từ xa qua Bluetooth**: Ứng dụng di động có thể kết nối với hệ thống để gửi lệnh mở khóa, xem nhật ký truy cập hoặc thực hiện cấu hình ban đầu.

---

### 4. Ưu điểm hệ thống

* 🛡️ **Bảo mật cao** nhờ sự kết hợp của nhiều hình thức xác thực và giới hạn số lần nhập sai.

* 🔧 **Dễ lắp đặt và tùy chỉnh**, sử dụng các linh kiện phổ biến và mã nguồn mở Arduino IDE.

* 🔋 **Tiết kiệm năng lượng** nhờ chế độ ngủ, chỉ kích hoạt khi có người đến gần.

* 📦 **Khả năng mở rộng cao**, có thể tích hợp thêm WiFi, camera hoặc kết nối với hệ thống quản lý thông minh.

---

### 5. Ứng dụng thực tiễn

* 🏠 **Nhà ở thông minh**: Đảm bảo an ninh và tiện lợi cho hộ gia đình.
* 🏢 **Văn phòng làm việc**: Quản lý quyền truy cập cho nhân viên.
* 🧪 **Phòng thiết bị/lab**: Kiểm soát nghiêm ngặt thiết bị và tài liệu chuyên dụng.
* 📦 **Kho hàng/nhà xưởng nhỏ**: Ngăn chặn truy cập trái phép và hỗ trợ giám sát từ xa.

---
