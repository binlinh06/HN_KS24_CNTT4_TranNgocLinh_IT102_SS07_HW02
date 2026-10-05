# BÁO CÁO THIẾT KẾ MODULE QUẢN LÝ CỤM THIẾT BỊ PHÒNG THÔNG MINH

## 1. Mục tiêu

Xây dựng chương trình C quản lý một thiết bị thông minh và một cảm biến điện năng trong hệ thống Smart Home.

Chương trình sử dụng hai cấu trúc `struct`:

* `SmartDevice`: lưu thông tin thiết bị.
* `EnergySensor`: lưu thông tin cảm biến.

Chương trình phải kiểm tra dữ liệu đầu vào, tự động ngắt thiết bị khi quá tải hoặc khi phòng vắng người, đồng thời tính điện năng và tiền điện theo biểu giá bậc thang.

## 2. Phân tích dữ liệu đầu vào

### 2.1. Struct SmartDevice

| Trường         | Kiểu dữ liệu | Ý nghĩa                       |
| -------------- | ------------ | ----------------------------- |
| `deviceID`     | `int`        | Mã thiết bị                   |
| `deviceName`   | `char[]`     | Tên thiết bị                  |
| `status`       | `int`        | Trạng thái: 1 bật, 0 tắt      |
| `ratedPowerKW` | `float`      | Công suất định mức, đơn vị kW |

### 2.2. Struct EnergySensor

| Trường               | Kiểu dữ liệu | Ý nghĩa                                 |
| -------------------- | ------------ | --------------------------------------- |
| `voltageVolts`       | `float`      | Điện áp đo được, đơn vị V               |
| `currentAmperes`     | `float`      | Dòng điện đo được, đơn vị A             |
| `unoccupancyMinutes` | `int`        | Thời gian phòng vắng người, đơn vị phút |

### 2.3. Biến phụ trợ

`operatingHours` lưu số giờ thiết bị được sử dụng trong tháng.

Ngoài ra chương trình sử dụng:

* `energyKWh`: điện năng tiêu thụ.
* `electricityCost`: tiền điện trước VAT.

## 3. Phân tích dữ liệu đầu ra

Chương trình xuất:

1. Mã và tên thiết bị.
2. Trạng thái thiết bị sau khi áp dụng các luật tự động hóa.
3. Điện áp và dòng điện cảm biến.
4. Thời gian vắng người.
5. Số giờ vận hành.
6. Tổng điện năng tiêu thụ.
7. Tổng tiền điện chưa bao gồm VAT.
8. Cảnh báo quá tải hoặc thông báo tự động tắt khi phòng vắng người.

## 4. Quy tắc kiểm tra dữ liệu

### 4.1. Dữ liệu cảm biến

Nếu một trong các giá trị sau nhỏ hơn 0:

* `sensor.voltageVolts`
* `sensor.currentAmperes`
* `sensor.unoccupancyMinutes`

thì dữ liệu bị xem là không hợp lệ.

Chương trình thông báo lỗi và dừng xử lý.

### 4.2. Thời gian vận hành

`operatingHours` phải thỏa mãn:

```text
0 < operatingHours <= 744
```

744 giờ tương ứng với 31 ngày × 24 giờ.

Nếu dữ liệu không hợp lệ, chương trình từ chối tính toán.

## 5. Kiểm soát an toàn điện

Dòng điện được truy xuất thông qua toán tử `.`:

```c
sensor.currentAmperes
```

Nếu:

```c
sensor.currentAmperes > 30.0f
```

thì hệ thống:

```c
device.status = 0;
```

Thiết bị được tự động tắt và hiển thị cảnh báo nguy cơ cháy nổ.

Luật quá tải được xử lý trước luật tiết kiệm năng lượng để đảm bảo an toàn điện được ưu tiên cao nhất.

## 6. Tự động tiết kiệm năng lượng

Nếu thiết bị vẫn đang bật:

```c
device.status == 1
```

và phòng vắng người ít nhất 15 phút:

```c
sensor.unoccupancyMinutes >= 15
```

thì hệ thống thực hiện:

```c
device.status = 0;
```

Thiết bị được tự động tắt để tiết kiệm năng lượng.

## 7. Tính điện năng tiêu thụ

Điện năng được tính theo công thức:

```text
kWh = ratedPowerKW × operatingHours
```

Trong chương trình:

```c
energyKWh = device.ratedPowerKW * operatingHours;
```

Thuộc tính của `struct` được truy xuất trực tiếp bằng toán tử `.`.

## 8. Tính tiền điện bậc thang

Biểu giá được áp dụng như sau:

| Bậc |     Sản lượng |     Đơn giá |
| --- | ------------: | ----------: |
| 1   |    0 - 50 kWh | 1.806 đ/kWh |
| 2   |  51 - 100 kWh | 1.866 đ/kWh |
| 3   | 101 - 200 kWh | 2.167 đ/kWh |
| 4   |  Trên 200 kWh | 2.729 đ/kWh |

Ví dụ nếu sản lượng là 150 kWh:

```text
50 × 1.806
+ 50 × 1.866
+ 50 × 2.167
```

Tiền điện được tính theo từng phần sản lượng thuộc mỗi bậc.

## 9. Trình tự xử lý

Chương trình thực hiện theo trình tự:

```text
Bước 1: Nhập thông tin SmartDevice
        ↓
Bước 2: Nhập thông tin EnergySensor
        ↓
Bước 3: Nhập operatingHours
        ↓
Bước 4: Kiểm tra dữ liệu cảm biến
        ↓
      Sai → Báo lỗi và kết thúc
        ↓
Bước 5: Kiểm tra operatingHours
        ↓
      Sai → Báo lỗi và kết thúc
        ↓
Bước 6: Kiểm tra dòng điện > 30A
        ↓
      Có → Tắt thiết bị + cảnh báo
        ↓
Bước 7: Kiểm tra phòng vắng ≥ 15 phút
        ↓
      Có → Tắt thiết bị
        ↓
Bước 8: Tính kWh
        ↓
Bước 9: Tính tiền điện bậc thang
        ↓
Bước 10: Xuất báo cáo
```

## 10. Áp dụng struct và toán tử `.`

Chương trình khai báo:

```c
struct SmartDevice device;
struct EnergySensor sensor;
```

Các thành phần được truy xuất bằng toán tử dấu chấm:

```c
device.deviceID
device.deviceName
device.status
device.ratedPowerKW

sensor.voltageVolts
sensor.currentAmperes
sensor.unoccupancyMinutes
```

Không sử dụng con trỏ struct, không sử dụng mảng struct và không tạo hàm tự định nghĩa.

## 11. Kết luận

Chương trình đáp ứng các yêu cầu của bài tập:

* Sử dụng `struct SmartDevice`.
* Sử dụng `struct EnergySensor`.
* Truy xuất thuộc tính bằng toán tử `.`.
* Không sử dụng mảng cấu trúc.
* Không sử dụng hàm tự định nghĩa.
* Kiểm tra dữ liệu cảm biến trước khi xử lý.
* Kiểm tra thời gian vận hành hợp lệ.
* Tự động ngắt khi dòng điện vượt 30A.
* Tự động tắt khi phòng vắng từ 15 phút.
* Tính điện năng theo công thức kWh.
* Tính tiền điện theo bậc thang.
* Xuất báo cáo trạng thái cuối cùng của thiết bị.
