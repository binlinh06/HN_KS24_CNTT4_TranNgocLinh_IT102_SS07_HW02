#include <stdio.h>

struct SmartDevice {
    int deviceID;
    char deviceName[100];
    int status;
    float ratedPowerKW;
};

struct EnergySensor {
    float voltageVolts;
    float currentAmperes;
    int unoccupancyMinutes;
};

int main() {
    struct SmartDevice device;
    struct EnergySensor sensor;

    float operatingHours;
    float energyKWh;
    float electricityCost = 0.0f;

    /* Nhap thong tin thiet bi */
    printf("=== NHAP THONG TIN THIET BI ===\n");

    printf("Ma thiet bi: ");
    scanf("%d", &device.deviceID);

    printf("Ten thiet bi: ");
    scanf(" %[^\n]", device.deviceName);

    printf("Trang thai (1-BAT, 0-TAT): ");
    scanf("%d", &device.status);

    printf("Cong suat dinh muc (kW): ");
    scanf("%f", &device.ratedPowerKW);

    /* Nhap thong tin cam bien */
    printf("\n=== NHAP THONG TIN CAM BIEN ===\n");

    printf("Dien ap (V): ");
    scanf("%f", &sensor.voltageVolts);

    printf("Dong dien (A): ");
    scanf("%f", &sensor.currentAmperes);

    printf("Thoi gian vang nguoi (phut): ");
    scanf("%d", &sensor.unoccupancyMinutes);

    /* Nhap thoi gian van hanh */
    printf("\nSo gio van hanh trong thang: ");
    scanf("%f", &operatingHours);

    /* ==========================================
       1. KIEM TRA DU LIEU CAM BIEN
       ========================================== */

    if (sensor.voltageVolts < 0 ||
        sensor.currentAmperes < 0 ||
        sensor.unoccupancyMinutes < 0) {

        printf("\n=== LOI DU LIEU ===\n");
        printf("Du lieu cam bien khong hop le!\n");
        printf("Dien ap, dong dien va thoi gian vang nguoi khong duoc am.\n");

        return 0;
    }

    /* ==========================================
       2. KIEM TRA THOI GIAN VAN HANH
       ========================================== */

    if (operatingHours <= 0 || operatingHours > 744) {

        printf("\n=== LOI DU LIEU ===\n");
        printf("So gio van hanh khong hop le!\n");
        printf("Phai lon hon 0 va khong vuot qua 744 gio/thang.\n");

        return 0;
    }

    /* ==========================================
       3. KIEM SOAT AN TOAN DIEN
       ========================================== */

    if (sensor.currentAmperes > 30.0f) {

        device.status = 0;

        printf("\n=== CANH BAO AN TOAN ===\n");
        printf("CANH BAO: Dong dien vuot qua 30A!\n");
        printf("Nguy co chay no. He thong da tu dong ngat thiet bi.\n");
    }

    /* ==========================================
       4. TIET KIEM NANG LUONG
       ========================================== */

    if (device.status == 1 &&
        sensor.unoccupancyMinutes >= 15) {

        device.status = 0;

        printf("\n=== TIET KIEM NANG LUONG ===\n");
        printf("Phong vang nguoi tu 15 phut tro len.\n");
        printf("He thong da tu dong tat thiet bi.\n");
    }

    /* ==========================================
       5. TINH DIEN NANG
       ========================================== */

    energyKWh = device.ratedPowerKW * operatingHours;

    /* ==========================================
       6. TINH TIEN DIEN BAC THANG
       ========================================== */

    if (energyKWh <= 50.0f) {

        electricityCost = energyKWh * 1806.0f;

    } else if (energyKWh <= 100.0f) {

        electricityCost =
            50.0f * 1806.0f
            + (energyKWh - 50.0f) * 1866.0f;

    } else if (energyKWh <= 200.0f) {

        electricityCost =
            50.0f * 1806.0f
            + 50.0f * 1866.0f
            + (energyKWh - 100.0f) * 2167.0f;

    } else {

        electricityCost =
            50.0f * 1806.0f
            + 50.0f * 1866.0f
            + 100.0f * 2167.0f
            + (energyKWh - 200.0f) * 2729.0f;
    }

    /* ==========================================
       7. XUAT BAO CAO
       ========================================== */

    printf("\n============================================\n");
    printf("       BAO CAO SMART HOME IOT\n");
    printf("============================================\n");

    printf("Ma thiet bi: %d\n", device.deviceID);
    printf("Ten thiet bi: %s\n", device.deviceName);

    printf("Trang thai: %s\n",
           device.status == 1 ? "BAT" : "TAT");

    printf("Cong suat dinh muc: %.2f kW\n",
           device.ratedPowerKW);

    printf("Dien ap: %.2f V\n",
           sensor.voltageVolts);

    printf("Dong dien: %.2f A\n",
           sensor.currentAmperes);

    printf("Thoi gian vang nguoi: %d phut\n",
           sensor.unoccupancyMinutes);

    printf("So gio van hanh: %.2f gio\n",
           operatingHours);

    printf("Dien nang tieu thu: %.2f kWh\n",
           energyKWh);

    printf("Tien dien (chua VAT): %.0f VND\n",
           electricityCost);

    printf("============================================\n");

    return 0;
}