#include "webserver.hpp"

WebServer server(80); // <-- This is the ONLY definition
HAL *hal_ptr;

// Web server handlers
void handleRoot();
void handleEnable();
void handleSlider();

void setupWebServer(HAL *hal)
{
    hal_ptr = hal;
    server.on("/", handleRoot);
    server.on("/enable", handleEnable);
    server.on("/slider", handleSlider);
    server.begin();
}

void handleEnable()
{
    if (hal_ptr->isEnabled())
        hal_ptr->disable();
    else
        hal_ptr->enable();

    server.sendHeader("Location", "/");
    server.send(303);
}

void handleSlider()
{
    if (server.hasArg("pin") && server.hasArg("value"))
    {
        int index = server.arg("pin").toInt();
        int value = server.arg("value").toInt();

        hal_ptr->applyPower(index, value);
    }

    server.sendHeader("Location", "/");
    server.send(303);
}
void handleRoot()
{
    String html =
R"rawliteral(
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body {
    font-family: sans-serif;
    text-align: center;
    background: #111;
    color: white;
}
h1 {
    margin-top: 20px;
}
.section {
    margin: 20px auto;
    padding: 20px;
    background: #1e1e1e;
    border-radius: 15px;
    width: 90%;
    max-width: 600px;
}
input[type=range] {
    width: 100%;
}
button {
    padding: 10px 20px;
    margin: 10px;
    font-size: 16px;
    border-radius: 10px;
    border: none;
    cursor: pointer;
}
.slider-container {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 10px;
}
.slider-label {
    width: 50px;
}
.slider-value {
    width: 40px;
    text-align: right;
}
</style>
</head>
<body>
<h1>MROBOSUB TEST CONTROL</h1>
<form action='/enable' method='get'>
<button style='background:)rawliteral";

    html += String(hal_ptr->isEnabled() ? "green" : "red");

    html += R"rawliteral(;color:white;'>)rawliteral";

    html += String(hal_ptr->isEnabled() ? "TESTING ENABLED" : "TESTING DISABLED");

    html +=
R"rawliteral(</button>
</form>
<div class='section'>
<h2>Outputs</h2>
)rawliteral";

    // Loop through sliders
    for (int i = 0; i < 8; i++)
    {
        html += "<div class='slider-container'>"
                "<span class='slider-label'>" +
                String(hal_ptr->getPinLabel(i)) + "</span>"
                                                  "<input type='range' min='-100' max='100' value='" +
                String(hal_ptr->getPinPercent(i)) +
                "' onchange='updateSlider(" + String(i) + ", this.value, this.nextElementSibling)' " +
                (hal_ptr->isEnabled() ? "" : "disabled") + ">"
                                                           "<span class='slider-value'>" +
                String(hal_ptr->getPinPercent(i)) + "%</span>"
                                                    "</div>";
    }

    html +=
R"rawliteral(
</div>
<script>
function updateSlider(pin, value, valueElem) {
    fetch(`/slider?pin=${pin}&value=${value}`);
    if(valueElem) {
        valueElem.innerText = value + '%';
    }
}
</script>
</body>
</html>
)rawliteral";

    server.send(200, "text/html", html);
}

void updateWebServer()
{
    server.handleClient();
}
