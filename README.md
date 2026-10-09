# 🔌 RK-Development 4-Channel Smart Control Board

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Language: C++](https://img.shields.io/badge/Language-C%2B%2B-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B)
[![Platform: ESP8266](https://img.shields.io/badge/Platform-ESP8266-orange.svg)](https://www.espressif.com/)

---

## 📋 Overview

**RK-Development 4-Channel** هي كارتة تحكم إلكترونية ذكية متطورة مخصصة لتطبيقات المنزل الذكي (Smart Home) وأنظمة التحكم الآلي (IoT/Embedded Systems).

تجمع الكارتة بين التحكم في الأحمال الكهربائية، والمفاتيح اليدوية، والحساسات البيئية، وإضاءة RGB - كل ذلك في تصميم واحد قابل للتطوير برمجيًا وهندسيًا.

![Project Banner](./docs/images/project-overview.png)

---

## ✨ المميزات الرئيسية

### 🎛️ التحكم الذكي
- **4 قنوات تحكم** للأحمال الكهربائية المستقلة
- **4 مفاتيح يدوية** مع دعم التحكم المدمج
- **تحكم RGB LED** بـ 256 درجة لكل لون
- **واجهة ويب** احترافية وتفاعلية

### 📊 المراقبة والحساسات
- **حساس DHT22** لقراءة درجة الحرارة والرطوبة
- **حساس الغاز MQ** للكشف عن الغازات
- **عرض فوري** للبيانات على لوحة التحكم

### 🔧 المرونة الهندسية
- **تصميم معياري** قابل للتوسع
- **ملفات PCB جاهزة** للتصنيع
- **كود مفتوح المصدر** يسهل التطوير
- **توثيق شامل** بالعربية والإنجليزية

### 🌐 الاتصالية
- **اتصال WiFi** عبر ESP8266
- **API REST** للتحكم عن بعد
- **واجهة ويب** محسّنة للهاتف والحاسب

---

## 📁 هيكل المشروع

```
RK-Development-4ch_/
├── README.md                          # هذا الملف
├── LICENSE                            # رخصة المشروع
│
├── /firmware                          # الكود البرمجي
│   ├── deepseek_cpp_20261009_f7fb90.cpp    # البرنامج الرئيسي
│   └── README_firmware.md             # شرح الكود
│
├── /hardware                          # ملفات التصميم الهندسي
│   ├── Gerber_RK-Development-4ch_PCB_RK-Development-4ch_2026-10-09.zip
│   │   ├── Gerber_TopLayer.GTL
│   │   ├── Drill_PTH_Through.DRL
│   │   ├── Drill_NPTH_Through.DRL
│   │   └── [ملفات الطبقات الأخرى]
│   ├── schematic/                     # ملفات الرسومات الكهربائية
│   └── PCB_design/                    # ملفات التصميم
│
├── /docs                              # التوثيق والشروحات
│   ├── INSTALLATION.md                # دليل التثبيت
│   ├── USAGE.md                       # دليل الاستخدام
│   ├── API_REFERENCE.md               # مرجع API
│   ├── PINOUT.md                      # توصيلات الأطراف
│   └── /images                        # الصور والرسوما��
│
├── /examples                          # أمثلة عملية
│   ├── basic_relay_control.cpp
│   ├── temperature_monitoring.cpp
│   └── web_dashboard_example.cpp
│
└── /screenshots                       # لقطات المشروع
    ├── Screenshot from 2026-10-09 12-40-41.png
    ├── Screenshot from 2026-10-09 12-41-51.png
    ├── Screenshot from 2026-10-09 13-01-16.png
    ├── ChatGPT Image Oct 9, 2026, 12_54_07 PM.png
    └── كارتة تحكم ذكية للتعلّم والتطوير.png
```

---

## 🚀 البدء السريع

### المتطلبات
- **متحكم**: ESP8266 (NodeMCU أو Wemos D1 Mini)
- **حساس الحرارة**: DHT22
- **حساس الغاز**: MQ-2 أو MQ-7
- **مكتبات Arduino**:
  - `ESP8266WiFi`
  - `ESP8266WebServer`
  - `DHT sensor library`

### التثبيت

#### 1️⃣ تثبيت مكتبات Arduino
```bash
# افتح Arduino IDE
# انتقل إلى: Sketch → Include Library → Manage Libraries
# ابحث عن:
- ESP8266WiFi
- DHT sensor library
- وثبتها
```

#### 2️⃣ رفع الكود
```bash
1. افتح ملف البرنامج الرئيسي:
   firmware/deepseek_cpp_20261009_f7fb90.cpp

2. عدّل بيانات الواي فاي:
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASS";

3. اختر لوحة ESP8266 من القائمة
   
4. اضغط Upload
```

#### 3️⃣ الوصول إلى لوحة التحكم
```
بعد رفع الكود بنجاح:
1. افتح Serial Monitor (Ctrl+Shift+M)
2. ستجد رسالة:
   "Connected! IP: 192.168.x.x"
3. انسخ عنوان IP في متصفح الويب
4. استمتع بلوحة التحكم! 🎉
```

---

## 📖 التوثيق الكامل

| الملف | الوصف |
|------|-------|
| [INSTALLATION.md](./docs/INSTALLATION.md) | شرح التثبيت والإعدادات |
| [USAGE.md](./docs/USAGE.md) | دليل الاستخدام والميزات |
| [API_REFERENCE.md](./docs/API_REFERENCE.md) | توثيق API والـ Endpoints |
| [PINOUT.md](./docs/PINOUT.md) | توصيلات الأطراف والدائرة |
| [TROUBLESHOOTING.md](./docs/TROUBLESHOOTING.md) | حل المشاكل الشائعة |

---

## 🔧 الأطراف والتوصيلات

### الريلايات (Relays)
```
RELAY_1 → GPIO14 (D5) → الإضاءة الرئيسية
RELAY_2 → GPIO12 (D6) → المروحة
RELAY_3 → GPIO13 (D7) → التلفاز
RELAY_4 → GPIO15 (D8) → مقبس ذكي
```

### المفاتيح اليدوية (Switches)
```
SW_1 → GPIO16 (D0)
SW_2 → GPIO0  (D3)
SW_3 → GPIO2  (D4)
SW_4 → GPIO4  (D2)
```

### الحساسات
```
DHT22    → GPIO2  (D4)
MQ_Sensor → A0 (ADC)
```

### إضاءة RGB
```
RGB_R → GPIO5  (D1)
RGB_G → GPIO4  (D2)
RGB_B → GPIO0  (D3)
```

📌 **ملاحظة**: قد يكون هناك تعارضات في بعض الأطراف. راجع [PINOUT.md](./docs/PINOUT.md) للتفاصيل الكاملة.

---

## 🌐 API والـ Endpoints

### الحصول على الحالة
```bash
GET /api/status
```
**الاستجابة:**
```json
{
  "temp": 25.5,
  "hum": 60.2,
  "gas": 450,
  "relays": [true, false, true, false]
}
```

### التحكم بالريلايات
```bash
GET /api/relay?num=0
```
التبديل بين ON/OFF للريلاي رقم 0-3

### التحكم بـ RGB
```bash
GET /api/rgb?r=255&g=128&b=0
```
ضبط الألوان (0-255 لكل لون)

---

## 📸 لقطات المشروع

### لوحة التحكم
![Dashboard Screenshot](./screenshots/Screenshot%20from%202026-10-09%2012-40-41.png)

### التصميم
![Design Screenshot](./screenshots/كارتة%20تحكم%20ذكية%20للتعلّم%20والتطوير.png)

### المزيد من اللقطات
- [Screenshot 2](./screenshots/Screenshot%20from%202026-10-09%2012-41-51.png)
- [Screenshot 3](./screenshots/Screenshot%20from%202026-10-09%2013-01-16.png)

---

## 💡 أمثلة عملية

### التحكم البسيط بالريلايات
```cpp
// تشغيل الريلاي الأول
setRelay(0, true);

// إيقاف الريلاي الثاني
setRelay(1, false);
```

### قراءة درجة الحرارة
```cpp
float temperature = dht.readTemperature();
Serial.println(temperature); // مثال: 25.5
```

### تغيير لون RGB
```cpp
// أحمر
analogWrite(RGB_R, 255);
analogWrite(RGB_G, 0);
analogWrite(RGB_B, 0);
```

### التحكم عن طريق الويب
```javascript
// من أي متصفح
fetch('/api/rgb?r=0&g=255&b=0'); // أخضر
fetch('/api/relay?num=0');        // تبديل الريلاي الأول
```

---

## 🛠️ التطوير والمساهمة

هذا المشروع مفتوح للتطوير والتحسين. إذا كنت تريد المساهمة:

1. **Fork** المشروع
2. **إنشئ فرع** جديد: `git checkout -b feature/improvement`
3. **قم بالتعديلات** والاختبارات
4. **Commit** التغييرات: `git commit -m "Add improvement"`
5. **Push** الفرع: `git push origin feature/improvement`
6. **فتح Pull Request**

### مجالات التطوير المقترحة
- [ ] إضافة دعم MQTT
- [ ] نظام حفظ وتحميل الإعدادات
- [ ] تطبيق موبايل للتحكم
- [ ] نظام التنبيهات والأتمتة
- [ ] إضافة وظائف أمان متقدمة

---

## ⚙️ تحسينات مستقبلية

- ✅ نسخة أساسية من الفيرموير
- ⏳ دعم اتصال MQTT
- ⏳ تطبيق موبايل native
- ⏳ نظام أتمتة ذكية
- ⏳ تكامل مع منصات Smart Home الشهيرة

---

## 📞 التواصل والدعم

### للأسئلة والاستفسارات
- **صفحة RKTRONIX**: [Facebook](https://facebook.com/RKTRONIX)
- **البريد الإلكتروني**: contact@rktronix.com
- **Issues & Discussions**: استخدم قسم Issues في المستودع

### خدمات RKTRONIX
- 🔌 تصميم الدوائر الإلكترونية
- 📐 تصميم وتصنيع PCB
- 💻 برمجة المتحكمات الدقيقة
- 🤖 تطوير حلول IoT وEmbedded Systems
- 🔄 إعادة تصميم وتحسين المنتجات

---

## 📜 الرخصة

هذا المشروع مرخص تحت [MIT License](./LICENSE)

يمكنك استخدام هذا المشروع بحرية للأغراض التعليمية والتجارية مع الحفاظ على الإشارة إلى الملكية الأصلية.

---

## 🎓 التعلم والموارد

### مواد تعليمية مفيدة
- [ESP8266 Documentation](https://arduino-esp8266.readthedocs.io/)
- [Arduino Getting Started](https://www.arduino.cc/en/Guide)
- [IoT Basics Tutorial](https://www.coursera.org/learn/iot)

### كتب موصى بها
- **"Arduino Projects"** - Scott Fitzgerald
- **"IoT for Everyone"** - Jaswinder Singh

---

## 📊 إحصائيات المشروع

![Language](https://img.shields.io/badge/C%2B%2B-100%25-blue)
![Status](https://img.shields.io/badge/Status-Active-green)
![Last Update](https://img.shields.io/badge/Last%20Update-October%202026-brightgreen)

---

## 🙏 شكر وتقدير

شكراً لاستخدامك هذا المشروع. نأمل أن يساهم في رحلتك التعليمية والمهنية.

**من فكرة هندسية إلى تصميم قابل للتطوير!**

---

**RKTRONIX** ©2026  
*Electronics • Embedded Systems • IoT*  
**From Idea to Intelligent Product.**

---

### آخر تحديث
تم آخر تحديث في **9 أكتوبر 2026**

للمزيد من المعلومات، تفضل بزيارة [المستودع الكامل](https://github.com/karim554/RK-Development-4ch_)
