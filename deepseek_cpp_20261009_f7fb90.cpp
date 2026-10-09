#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

// ==========================================
// 1. إعدادات الواي فاي (قم بتغييرها لبيانات شبكتك)
// ==========================================
const char* ssid = "YOUR_WIFI_SSID";      // اسم شبكة الواي فاي
const char* password = "YOUR_WIFI_PASS";  // باسورد الواي فاي

// ==========================================
// 2. تعريف الأطراف (Pins) بناءً على الـ Schematic
// ==========================================
// الريلايات (Relays) - الـ ULN2003A يعمل بنظام Active LOW
#define RELAY_1 14 // IO14 (D5)
#define RELAY_2 12 // IO12 (D6)
#define RELAY_3 13 // IO13 (D7)
#define RELAY_4 15 // IO15 (D8)

// المفاتيح اليدوية (Switches) - Active LOW
#define SW_1 16    // IO16 (D0)
#define SW_2 0     // IO0  (D3)
#define SW_3 2     // IO2  (D4)
#define SW_4 4     // IO4  (D2)

// إضاءة RGB
#define RGB_R 5    // IO5 (D1)
#define RGB_G 4    // IO4 (D2) - (ملاحظة: قد يتعارض مع SW_4 في بعض التصميمات)
#define RGB_B 0    // IO0 (D3) - (ملاحظة: قد يتعارض مع SW_2 في بعض التصميمات)

// الحساسات
#define DHTPIN 2   // IO2 (D4) - متصل بحساس DHT22
#define DHTTYPE DHT22
#define MQ_PIN A0  // مدخل حساس الغاز

// ==========================================
// 3. المتغيرات العامة
// ==========================================
ESP8266WebServer server(80);
DHT dht(DHTPIN, DHTTYPE);

bool relayState[4] = {false, false, false, false};
bool lastSwitchState[4] = {HIGH, HIGH, HIGH, HIGH};
unsigned long lastDebounceTime[4] = {0, 0, 0, 0};
const unsigned long debounceDelay = 50;

// دالة مساعدة لقلب حالة الريلاي (لأنه Active LOW)
void setRelay(int relayNum, bool state) {
    int pin = (relayNum == 0) ? RELAY_1 : (relayNum == 1) ? RELAY_2 : (relayNum == 2) ? RELAY_3 : RELAY_4;
    digitalWrite(pin, state ? LOW : HIGH); // LOW يشغل الريلاي
    relayState[relayNum] = state;
}

// ==========================================
// 4. كود صفحة الويب (HTML/CSS/JS) المدمج - التصميم الاحترافي
// ==========================================
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html lang="ar" dir="rtl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>RKTRONIX | Smart Home Dashboard</title>
    <script src="https://cdn.tailwindcss.com"></script>
    <link href="https://fonts.googleapis.com/css2?family=Tajawal:wght@300;400;500;700;800&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
    <style>
        body { font-family: 'Tajawal', sans-serif; background-color: #0B1120; background-image: radial-gradient(circle at 10% 20%, rgba(56, 189, 248, 0.05) 0%, transparent 20%), radial-gradient(circle at 90% 80%, rgba(168, 85, 247, 0.05) 0%, transparent 20%); color: #f8fafc; min-height: 100vh; }
        .glass-card { background: rgba(30, 41, 59, 0.4); backdrop-filter: blur(12px); -webkit-backdrop-filter: blur(12px); border: 1px solid rgba(255, 255, 255, 0.08); border-radius: 20px; transition: all 0.3s ease; }
        .glass-card:hover { border-color: rgba(56, 189, 248, 0.3); box-shadow: 0 0 20px rgba(56, 189, 248, 0.1); }
        .switch { position: relative; display: inline-block; width: 50px; height: 26px; }
        .switch input { opacity: 0; width: 0; height: 0; }
        .slider { position: absolute; cursor: pointer; top: 0; left: 0; right: 0; bottom: 0; background-color: #334155; transition: .4s; border-radius: 34px; }
        .slider:before { position: absolute; content: ""; height: 20px; width: 20px; left: 3px; bottom: 3px; background-color: white; transition: .4s; border-radius: 50%; }
        input:checked + .slider { background-color: #10b981; box-shadow: 0 0 10px #10b981; }
        input:checked + .slider:before { transform: translateX(24px); }
        input[type=range] { -webkit-appearance: none; width: 100%; background: transparent; }
        input[type=range]::-webkit-slider-thumb { -webkit-appearance: none; height: 20px; width: 20px; border-radius: 50%; background: #fff; cursor: pointer; margin-top: -8px; box-shadow: 0 0 10px rgba(255,255,255,0.5); }
        input[type=range]::-webkit-slider-runnable-track { width: 100%; height: 4px; cursor: pointer; background: #334155; border-radius: 2px; }
        .range-red::-webkit-slider-thumb { box-shadow: 0 0 10px #ef4444; }
        .range-green::-webkit-slider-thumb { box-shadow: 0 0 10px #10b981; }
        .range-blue::-webkit-slider-thumb { box-shadow: 0 0 10px #3b82f6; }
        .pulse-dot { width: 10px; height: 10px; background-color: #10b981; border-radius: 50%; display: inline-block; box-shadow: 0 0 0 0 rgba(16, 185, 129, 0.7); animation: pulse 2s infinite; }
        @keyframes pulse { 0% { transform: scale(0.95); box-shadow: 0 0 0 0 rgba(16, 185, 129, 0.7); } 70% { transform: scale(1); box-shadow: 0 0 0 10px rgba(16, 185, 129, 0); } 100% { transform: scale(0.95); box-shadow: 0 0 0 0 rgba(16, 185, 129, 0); } }
    </style>
</head>
<body class="p-4 md:p-8">
    <header class="flex justify-between items-center mb-8 glass-card p-4 px-6">
        <div class="flex items-center gap-3">
            <div class="bg-blue-600 p-2 rounded-lg text-white"><i class="fa-solid fa-microchip text-xl"></i></div>
            <div><h1 class="text-xl font-bold tracking-wide">RKTRONIX</h1><p class="text-xs text-gray-400">نظام التحكم الذكي</p></div>
        </div>
        <div class="flex items-center gap-4">
            <div class="flex items-center gap-2 bg-gray-800 px-3 py-1 rounded-full text-sm border border-gray-700">
                <span class="pulse-dot"></span><span class="text-gray-300">متصل بالشبكة</span>
            </div>
        </div>
    </header>

    <main class="grid grid-cols-1 lg:grid-cols-3 gap-6 max-w-7xl mx-auto">
        <div class="lg:col-span-2 space-y-6">
            <div class="glass-card p-6">
                <div class="flex justify-between items-center mb-6">
                    <h2 class="text-lg font-bold text-gray-200"><i class="fa-solid fa-plug text-blue-400 ml-2"></i> الأحمال الكهربائية</h2>
                </div>
                <div class="grid grid-cols-1 md:grid-cols-2 gap-4">
                    <!-- Relay 1 -->
                    <div class="bg-gray-800/50 p-4 rounded-xl border border-gray-700 flex justify-between items-center">
                        <div class="flex items-center gap-3">
                            <div class="w-12 h-12 rounded-full bg-yellow-500/20 flex items-center justify-center text-yellow-400"><i class="fa-solid fa-lightbulb text-xl"></i></div>
                            <div><h3 class="font-bold text-gray-200">الإضاءة الرئيسية</h3><p class="text-xs text-gray-400 status-text" id="status-0">مطفأ</p></div>
                        </div>
                        <label class="switch"><input type="checkbox" id="switch-0" onchange="toggleRelay(0)"><span class="slider"></span></label>
                    </div>
                    <!-- Relay 2 -->
                    <div class="bg-gray-800/50 p-4 rounded-xl border border-gray-700 flex justify-between items-center">
                        <div class="flex items-center gap-3">
                            <div class="w-12 h-12 rounded-full bg-blue-500/20 flex items-center justify-center text-blue-400"><i class="fa-solid fa-fan text-xl"></i></div>
                            <div><h3 class="font-bold text-gray-200">المروحة</h3><p class="text-xs text-gray-400 status-text" id="status-1">مطفأ</p></div>
                        </div>
                        <label class="switch"><input type="checkbox" id="switch-1" onchange="toggleRelay(1)"><span class="slider"></span></label>
                    </div>
                    <!-- Relay 3 -->
                    <div class="bg-gray-800/50 p-4 rounded-xl border border-gray-700 flex justify-between items-center">
                        <div class="flex items-center gap-3">
                            <div class="w-12 h-12 rounded-full bg-purple-500/20 flex items-center justify-center text-purple-400"><i class="fa-solid fa-tv text-xl"></i></div>
                            <div><h3 class="font-bold text-gray-200">التلفاز</h3><p class="text-xs text-gray-400 status-text" id="status-2">مطفأ</p></div>
                        </div>
                        <label class="switch"><input type="checkbox" id="switch-2" onchange="toggleRelay(2)"><span class="slider"></span></label>
                    </div>
                    <!-- Relay 4 -->
                    <div class="bg-gray-800/50 p-4 rounded-xl border border-gray-700 flex justify-between items-center">
                        <div class="flex items-center gap-3">
                            <div class="w-12 h-12 rounded-full bg-green-500/20 flex items-center justify-center text-green-400"><i class="fa-solid fa-plug text-xl"></i></div>
                            <div><h3 class="font-bold text-gray-200">مقبس ذكي</h3><p class="text-xs text-gray-400 status-text" id="status-3">مطفأ</p></div>
                        </div>
                        <label class="switch"><input type="checkbox" id="switch-3" onchange="toggleRelay(3)"><span class="slider"></span></label>
                    </div>
                </div>
            </div>

            <div class="glass-card p-6">
                <h2 class="text-lg font-bold text-gray-200 mb-6"><i class="fa-solid fa-palette text-pink-400 ml-2"></i> إضاءة RGB</h2>
                <div class="flex flex-col md:flex-row gap-8 items-center">
                    <div class="relative w-32 h-32 rounded-full border-4 border-gray-700 flex items-center justify-center transition-all duration-300" id="colorPreview" style="background: #000; box-shadow: 0 0 30px rgba(0,0,0,0.5);"><i class="fa-solid fa-lightbulb text-4xl text-white/20"></i></div>
                    <div class="flex-1 w-full space-y-4">
                        <div><div class="flex justify-between text-xs text-gray-400 mb-1"><span>الأحمر (R)</span><span id="valR">0</span></div><input type="range" min="0" max="255" value="0" class="range-red" oninput="setRGB()" id="r"></div>
                        <div><div class="flex justify-between text-xs text-gray-400 mb-1"><span>الأخضر (G)</span><span id="valG">0</span></div><input type="range" min="0" max="255" value="0" class="range-green" oninput="setRGB()" id="g"></div>
                        <div><div class="flex justify-between text-xs text-gray-400 mb-1"><span>الأزرق (B)</span><span id="valB">0</span></div><input type="range" min="0" max="255" value="0" class="range-blue" oninput="setRGB()" id="b"></div>
                    </div>
                </div>
            </div>
        </div>

        <div class="space-y-6">
            <div class="glass-card p-6 h-full">
                <h2 class="text-lg font-bold text-gray-200 mb-6"><i class="fa-solid fa-temperature-half text-orange-400 ml-2"></i> المراقبة البيئية</h2>
                <div class="space-y-6">
                    <div class="flex items-center gap-4 bg-gray-800/50 p-4 rounded-xl border border-gray-700">
                        <div class="w-14 h-14 rounded-full bg-orange-500/20 flex items-center justify-center text-orange-400 text-2xl"><i class="fa-solid fa-temperature-high"></i></div>
                        <div><p class="text-sm text-gray-400">درجة الحرارة</p><p class="text-2xl font-bold text-white"><span id="temp">--</span> <span class="text-sm text-gray-500">°C</span></p></div>
                    </div>
                    <div class="flex items-center gap-4 bg-gray-800/50 p-4 rounded-xl border border-gray-700">
                        <div class="w-14 h-14 rounded-full bg-cyan-500/20 flex items-center justify-center text-cyan-400 text-2xl"><i class="fa-solid fa-droplet"></i></div>
                        <div><p class="text-sm text-gray-400">الرطوبة</p><p class="text-2xl font-bold text-white"><span id="hum">--</span> <span class="text-sm text-gray-500">%</span></p></div>
                    </div>
                    <div class="flex items-center gap-4 bg-gray-800/50 p-4 rounded-xl border border-gray-700">
                        <div class="w-14 h-14 rounded-full bg-red-500/20 flex items-center justify-center text-red-400 text-2xl"><i class="fa-solid fa-fire"></i></div>
                        <div><p class="text-sm text-gray-400">حساس الغاز (MQ)</p><p class="text-2xl font-bold text-white"><span id="gas">--</span></p></div>
                    </div>
                </div>
            </div>
        </div>
    </main>

    <script>
        function toggleRelay(num) {
            fetch('/api/relay?num=' + num).then(res => res.text()).then(state => { updateRelayUI(num, state === '1'); });
        }
        function updateRelayUI(num, isOn) {
            let sw = document.getElementById('switch-' + num);
            let txt = document.getElementById('status-' + num);
            sw.checked = isOn;
            if(isOn) { txt.innerText = "يعمل الآن"; txt.classList.add('text-green-400'); txt.classList.remove('text-gray-400'); } 
            else { txt.innerText = "مطفأ"; txt.classList.add('text-gray-400'); txt.classList.remove('text-green-400'); }
        }
        function setRGB() {
            let r = document.getElementById('r').value, g = document.getElementById('g').value, b = document.getElementById('b').value;
            document.getElementById('valR').innerText = r; document.getElementById('valG').innerText = g; document.getElementById('valB').innerText = b;
            let color = `rgb(${r}, ${g}, ${b})`;
            let preview = document.getElementById('colorPreview');
            preview.style.backgroundColor = color;
            preview.style.boxShadow = (r == 0 && g == 0 && b == 0) ? '0 0 30px rgba(0,0,0,0.5)' : `0 0 40px ${color}`;
            fetch(`/api/rgb?r=${r}&g=${g}&b=${b}`);
        }
        setInterval(() => {
            fetch('/api/status').then(res => res.json()).then(data => {
                document.getElementById('temp').innerText = data.temp;
                document.getElementById('hum').innerText = data.hum;
                document.getElementById('gas').innerText = data.gas;
                for(let i=0; i<4; i++) { updateRelayUI(i, data.relays[i]); }
            }).catch(e => console.log("Waiting for data..."));
        }, 2000);
    </script>
</body>
</html>
)rawliteral";

// ==========================================
// 5. إعدادات السيرفر والـ API
// ==========================================
void setup() {
    Serial.begin(115200);
    
    // تهيئة الأطراف
    pinMode(RELAY_1, OUTPUT); pinMode(RELAY_2, OUTPUT);
    pinMode(RELAY_3, OUTPUT); pinMode(RELAY_4, OUTPUT);
    setRelay(0, false); setRelay(1, false); setRelay(2, false); setRelay(3, false); 

    pinMode(SW_1, INPUT_PULLUP); pinMode(SW_2, INPUT_PULLUP);
    pinMode(SW_3, INPUT_PULLUP); pinMode(SW_4, INPUT_PULLUP);

    pinMode(RGB_R, OUTPUT); pinMode(RGB_G, OUTPUT); pinMode(RGB_B, OUTPUT);
    analogWriteRange(255); 

    dht.begin();

    // الاتصال بالواي فاي
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected! IP: ");
    Serial.println(WiFi.localIP());

    // مسارات السيرفر (Routes)
    server.on("/", []() { server.send(200, "text/html", htmlPage); });

    server.on("/api/relay", []() {
        if (server.hasArg("num")) {
            int num = server.arg("num").toInt();
            if (num >= 0 && num < 4) {
                setRelay(num, !relayState[num]); 
                server.send(200, "text/plain", relayState[num] ? "1" : "0");
                return;
            }
        }
        server.send(400, "text/plain", "Error");
    });

    server.on("/api/rgb", []() {
        if (server.hasArg("r") && server.hasArg("g") && server.hasArg("b")) {
            analogWrite(RGB_R, server.arg("r").toInt());
            analogWrite(RGB_G, server.arg("g").toInt());
            analogWrite(RGB_B, server.arg("b").toInt());
            server.send(200, "text/plain", "OK");
            return;
        }
        server.send(400, "text/plain", "Error");
    });

    server.on("/api/status", []() {
        float t = dht.readTemperature();
        float h = dht.readHumidity();
        int gas = analogRead(MQ_PIN);
        
        if (isnan(t)) t = 0.0;
        if (isnan(h)) h = 0.0;

        String json = "{";
        json += "\"temp\":" + String(t, 1) + ",";
        json += "\"hum\":" + String(h, 1) + ",";
        json += "\"gas\":" + String(gas) + ",";
        json += "\"relays\":[" + String(relayState[0]) + "," + String(relayState[1]) + "," + String(relayState[2]) + "," + String(relayState[3]) + "]";
        json += "}";
        
        server.send(200, "application/json", json);
    });

    server.begin();
    Serial.println("HTTP Server Started");
}

// ==========================================
// 6. الحلقة الرئيسية (Loop)
// ==========================================
void loop() {
    server.handleClient(); 

    // قراءة المفاتيح اليدوية (مع مانع الارتداد Debounce)
    int swPins[4] = {SW_1, SW_2, SW_3, SW_4};
    for (int i = 0; i < 4; i++) {
        int reading = digitalRead(swPins[i]);
        if (reading != lastSwitchState[i]) {
            lastDebounceTime[i] = millis();
        }
        if ((millis() - lastDebounceTime[i]) > debounceDelay) {
            if (reading == LOW && relayState[i] == false) {
                setRelay(i, true);
            } else if (reading == HIGH && relayState[i] == true) {
                setRelay(i, false);
            }
        }
        lastSwitchState[i] = reading;
    }
}