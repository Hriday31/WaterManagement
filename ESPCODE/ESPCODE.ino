#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

//const char *ssid = "A.T.O.M_LABS";
//const char *password = "Atom281121";
//const char *ssid = "Hriday";
//const char *password = "9953777876";
const char *ssid = "water1";
const char *password = "aquasquad";

int ENA_pin = 14;
int IN1 = 27;
int IN2 = 26;
int p=0;
const int frequency = 500;
const int pwm_channel = 0;
const int resolution = 8;

const char *input_parameter = "value";

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8" />
  <meta http-equiv="X-UA-Compatible" content="IE=edge" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Valve Control</title>
  <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css" rel="stylesheet"
    integrity="sha384-rbsA2VBKQhggwzxH7pPCaAqO46MgnOM80zW1RWuH61DGLwZJEdK2Kadq2F9CUG65" crossorigin="anonymous" />


  <style>
    * {
      font-size: 17px;
      font-family: "Lucida Sans", "Lucida Sans Regular", "Lucida Grande",
        "Lucida Sans Unicode", Geneva, Verdana, sans-serif;
      font-weight: 600;
      color: #1a1a1a;
    }

    html {
      padding: 20px;
      background-color: #095376;
    }

    body {
      background-color: #FFFCF5;
      border-radius: 20px;
      height: 95vh;
    }

    .a {
      /* on off*/
      margin: 7px;
    }

    .b {
      /* on off plus minus */
      background-color: #FFB875;
      border: none;
      border-radius: 50%;
      height: 3rem;
      width: 3rem;
    }

    .c {
      /* plus minus 100 */
      background-color: #FFB875;
      height: 100px;
      padding: 10px;
    }

    .center {
      display: flex;
      align-items: center;
      justify-content: center;
    }

    .d {
      /* 100 only */
      display: flex;
      align-items: center;
      justify-content: center;
      padding-top: 10px;
      /* margin-top: 10px; */
      font-size: x-large;
    }

    .e {
      display: flex;
      justify-content: end;
      align-items: center;
    }

    .topp {
      background-color: #FFB875;
      height: 60px;
      width: 130px;
      padding: 2px;
      margin-top: 2rem;
      margin-right: 1rem;
    }

    .navbtn {
      background-color: transparent;
      font-size: x-large;
      border: none;
      border-radius: 50%;
      height: 3rem;
      width: 3rem;
      margin: 0.1rem;
    }

    .normal {
      font-size: x-large;
    }

    button.navbtn:hover,
    button.navbtn:active {
      background-color: #FFFCF5;
    }

    button.a:hover {
      background-color: #FFFCF5;
    }

    button.normal {
      background-color: #FFFCF5;
    }
  </style>
</head>

<body>
  <div class="container-fluid">
    <div class="row align-content-center" style="height: 10vh">
      <div class="col-6"></div>
      <div class="col-6 d-inline-flex justify-content-end">
        <div class="rounded-pill topp d-inline-flex justify-content-around">
          <button class="navbtn normal">⚙️</button>
          <a href="/water"><button class="navbtn">🔥</button></a>
        </div>
      </div>
    </div>
    <div class="row align-content-center " style="height: 17vh">
      <div class="row">
        <div class="col-12 text-center ">
          <h2 style="font-weight: 700;margin-bottom: 0px;font-size: 26px;">Water Management Portal</h2>
        </div>
      </div>
      <div class="row">
        <div class="col-8 col-sm-12 text-center">
          <h4 style="font-weight: 1000; font-size: 22px;">Valve Control Unit</h4>
        </div>
      </div>
    </div>
    <div class="row align-content-center" style="height: 10vh">
      <div class="col-3 col-sm-4 e">
        <button type="button" class="button b center a" onclick="updateSliderPWMon()">ON</button>
      </div>
      <div class="col-6 col-sm-4 d-flex justify-content-around rounded-pill c">
        <div class="col-4 center">
          <button type="button" class="b" onclick="updateSliderPWMlow()" style="background-color:#FFFCF5 ;">➖</button>
        </div>
        <div class="col-4">
          <div class="d">
            <div class="d" id="textslider_value">100</div>
            <div style="font-size:large;padding-top: 10px;">%</div>
          </div>
        </div>
        <div class="col-4 center">
          <button type="button" class="b" onclick="updateSliderPWM()" style="background-color:#FFFCF5">➕</button>
        </div>
      </div>
      <div class="col-3 col-sm-4 e d-flex justify-content-start">
        <button type="button" class="button b center a" onclick="updateSliderPWMoff()">OFF</button>
      </div>
    </div>
    <div class="row align-content-end" style="height: 60vh">
      <div class="col-12 d-flex justify-content-center"><img
          src="https://media.istockphoto.com/id/872714390/vector/water-pouring-from-pipe-flow-of-clean-water-vector-illustration.jpg?s=170667a&w=0&k=20&c=g_NOjGQfV40Hp1d5FUXDUJsWhKfqj2I52xW-TJD633o="
          style="height: 45vh; border-radius: 30px 30px 0px 0px;" class="align-bottom"></div>
    </div>
  </div>
  <script>
    src = "https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/js/bootstrap.bundle.min.js"
    integrity = "sha384-kenU1KFdBIe4zVF0s0G1M5b4hcpxyD9F7jL+jjXkk+Q2h455rYXK/7HAuoJl+0I4"
    crossorigin = "anonymous"
    function updateSliderPWM() {
      var slider_value = document.getElementById("textslider_value").innerText;
      if (slider_value != 100) {
        slider_value = Number(slider_value) + 20;
      }
      document.getElementById("textslider_value").innerHTML = slider_value;
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/slider", true);
      xhr.send();
    }
    function updateSliderPWMlow() {
      var slider_value = document.getElementById("textslider_value").innerText;
      if (slider_value != 0) {
        slider_value = Number(slider_value) - 20;
      }
      document.getElementById("textslider_value").innerHTML = slider_value;
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/sliderlow", true);
      xhr.send();
    }
    function updateSliderPWMon() {
      d = new Date();
      t = d.getTime();
      console.log(t);
      v = document.getElementById('textslider_value')
      v.innerHTML = 100
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/slideron", true);
      xhr.send();
    }
    function updateSliderPWMoff() {
      v = document.getElementById('textslider_value')
      v.innerHTML = 0
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/slideroff", true);
      xhr.send();
    }
    function updateSliderPWMoff2() {
      updateSliderPWMoff();
      d2 = new Date();
      t2 = d2.getTime();
      console.log(t2);
      t3 = (t2 - t) / 1000;
      console.log(t3 * 25);
      document.getElementById("time").innerHTML = t3 * 25;

    }
  </script>
</body>

</html>
)rawliteral";
const char water_html[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="en" dir="ltr">
<head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css" rel="stylesheet"
        integrity="sha384-rbsA2VBKQhggwzxH7pPCaAqO46MgnOM80zW1RWuH61DGLwZJEdK2Kadq2F9CUG65" crossorigin="anonymous" />
    <title>Fire Control </title>
    <style>
        html {padding: 20px;
            background-color: #eee5e5;
        }

        body {
            border-radius: 20px;
            height: 95vh;
            background: black;
        }

        .ring {
            display: flex;
            justify-content: center;
            align-items: center;
        }

        .loader {
            position: relative;
            width: 300px;
            height: 300px;
            border-radius: 50%;
            background: linear-gradient(#000, #000000, #000);
            animation: animate 1s linear infinite;
        }

        @keyframes animate {
            0% {
                transform: rotate(0deg);
            }

            100% {
                transform: rotate(360deg);
            }
        }

        .loader span {
            position: absolute;
            width: 300px;
            height: 300px;
            border-radius: 50%;
            background: linear-gradient(#ff0000, black, #ff0000);
        }

        .loader span:nth-child(1) {
            filter: blur(20px);
        }

        .loader span:nth-child(2) {
            filter: blur(40px);
        }

        .loader::after {
            content: "";
            position: absolute;
            top: 10px;
            left: 10px;
            right: 10px;
            bottom: 10px;
            background: black;
            border-radius: 50%;
        }

        .topp {
            /* background-color: #ec2731; */
            height: 60px;
            width: 130px;
            padding: 2px;
            margin-top: 2rem;
            margin-right: 1rem;
        }

        .navbtn {
            background-color: transparent;
            font-size: x-large;
            border: none;
            border-radius: 50%;
            height: 3rem;
            width: 3rem;
            margin: 0.1rem;
        }

        .normal {
            font-size: x-large;
        }

        button.navbtn:hover,
        button.navbtn:active {
            background-color: #fff;
        }

        button.normal {
            background-color: #fff;
        }

        #circle {
            border: none;
            display: flex;
            justify-content: center;
            align-items: center;
            background-color: #ec2731;
            width: 250px;
            height: 250px;
            margin-top: -275px;
            z-index: 10;
            color: #fff;
            border-radius: 100%;
        }
    </style>
</head>

<body>
    <div class="container-fluid">
        <div class="row align-content-center" style="height: 10vh">
            <div class="col-6"></div>
            <div class="col-6 d-inline-flex justify-content-end">
                <div class="rounded-pill topp d-inline-flex justify-content-around">
                    <a href="/"><button class="navbtn ">⚙️</button></a>
                    <button class="navbtn normal">🔥</button>
                </div>
            </div>
        </div>
        <div class="row align-content-center" style="height: 20vh;"></div>
        <div class="row d-flex justify-content-center align-content-center">
            <div class="ring">
                <div class="loader">
                    <span class="sp"></span>
                    <span class="sp"></span>
                    <span class="sp"></span>
                    <span class="sp"></span>
                </div>

            </div>
            <button id="circle" onclick="change()">
                <h1 id="a">EMERGENCY</h1>
            </button>

        </div>
    </div>
    <script>
        src = "https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/js/bootstrap.bundle.min.js"
        integrity = "sha384-kenU1KFdBIe4zVF0s0G1M5b4hcpxyD9F7jL+jjXkk+Q2h455rYXK/7HAuoJl+0I4"
        crossorigin = "anonymous"
        i = 0;
        function change() {
            if (i % 2 == 0) {
                button = document.getElementById("circle");
                a = document.getElementById("a");
                button.style.backgroundColor = "#0879b6";
                a.innerText = "STOP";
                htmlcolor=document.querySelector("html");
                htmlcolor.style.backgroundColor="aliceblue";
                i++;
                var ele = document.getElementsByClassName('sp');
                for (var j = 0; j < ele.length; j++) {
                    ele[j].style.background = "linear-gradient(#0f5e9c, #1ca3ec, #0f5e9c)";
                }
                updateSliderPWMon();
            }
            else {updateSliderPWMoff();
                location.reload();
                i++;
            }
        }
        function updateSliderPWMon() {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/slideron", true);
      xhr.send();
    }
    function updateSliderPWMoff() {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/slideroff", true);
      xhr.send();
    }
    </script>
</body>

</html>
)rawliteral";

void setup()
{
  Serial.begin(115200);
  delay(1000);
  pinMode(ENA_pin, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  ledcSetup(pwm_channel, frequency, resolution);
  ledcAttachPin(ENA_pin, pwm_channel);
  ledcWrite(pwm_channel, p);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html); });
  server.on("/water", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", water_html); });
  server.on("/slider", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              if (p<255){
                p=p+51;
                ledcWrite(pwm_channel, p);
              }
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);

    Serial.println(p);
    request->send(200, "text/plain", "OK"); });

  server.on("/sliderlow", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              if (p>50){
                p=p-51;
                ledcWrite(pwm_channel, p);
              }
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);

    Serial.println(p);
    request->send(200, "text/plain", "OK"); });

  server.on("/slideron", HTTP_GET, [](AsyncWebServerRequest *request)
            {p=255;
              ledcWrite(pwm_channel, p);
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);

    Serial.println(p);
    request->send(200, "text/plain", "OK"); });

  server.on("/slideroff", HTTP_GET, [](AsyncWebServerRequest *request)
            {p=0;
              ledcWrite(pwm_channel, p);
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);

    Serial.println(p);
    request->send(200, "text/plain", "OK"); });

  server.begin();
}

void loop()
{
}