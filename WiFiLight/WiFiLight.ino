#include <ESP8266WiFi.h>

const char* ssid = "Mi10000";//WiFi名称
const char* password = "m.1522510849";//WiFi密码

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(2, OUTPUT);//2号脚位定为输出，即D4脚位
  digitalWrite(2, 1);
  
  Serial.println();
  Serial.print("连接到 Wi-Fi网络：");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wi-Fi连接成功！");

  Serial.print("地址为：");
  Serial.println(WiFi.localIP());
  
  server.begin();  // 启动服务器
  Serial.println("服务器已启动，等待连接...");
}

void loop() {
  WiFiClient client = server.available();  // 检查是否有客户端连接

  if (client) {
    Serial.println("新的客户端连接！等待新的指令...");
    
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');  // 读取客户端发送的请求
        //Serial.println(request);//打印出收到的讯息
        client.flush();

        int val;
        if (request.indexOf("/on") != -1) {
          // 收到开灯请求
          val = 0;
          Serial.println("开灯！");
        }
        else if (request.indexOf("/off") != -1) {
          // 收到关灯请求
          val = 1;
          Serial.println("关灯！");
        }

          digitalWrite(2, val);
  
          client.flush();
        
        // 返回响应给客户端
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();
        client.println("<!DOCTYPE HTML>");
        client.println("<html>");
        client.println("<head>");
        client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
        client.println("<style>");
        client.println("body { text-align: center; margin-top: 50px; }");
        client.println("h1 { font-size: 36px; }");
        client.println("button { font-size: 24px; padding: 10px 20px; }");
        client.println("</style>");
        client.println("</head>");
        client.println("<body>");
        client.println("<h1>Wi-Fi灯控制</h1>");
        client.println("<p><a href=\"/on\"><button style=\"background-color: green; color: white;\">开灯</button></a></p>");
        client.println("<p><a href=\"/off\"><button style=\"background-color: red; color: white;\">关灯</button></a></p>");
        client.println("</body>");
        client.println("</html>");
        break;
      }
    }
    
    // 延迟一段时间等待客户端断开连接
    delay(1000);
    client.stop();
    Serial.println("指令完成！客户端断开连接");
  }
}
