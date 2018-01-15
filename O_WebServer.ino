void WebServer()  {
  // listen for incoming clients
  EthernetClient client = server.available();
  // Serial.println("new client???");
  if (client) {
   // Serial.println("new client");
    char clientline[BUFSIZ];
    int index = 0;
    ;
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c != '\n' && c != '\r') {
          clientline[index] = c;
          index++;
          // are we too big for the buffer? start tossing out data
          if (index >= BUFSIZ)
            index = BUFSIZ - 1;

          // continue to read more data!
          continue;
        }

//        Serial.print ("clientLine: ");
//        Serial.println(clientline);

        if (strstr(clientline, "GET / ") != 0) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json;charset=utf-8");
          client.println("Server: Arduino");
          client.println("Connnection: close");
          client.println();
          client.print("{\"stats\":{\"current\":{");
          client.print ("\"outTemp\":");
          client.print(outTemp);
          client.print(",");
          client.print ("\"Temp5\":");
          client.print(Temp5);
          client.print(",");
          client.print ("\"Hum5\":");
          client.print(Hum5);
          client.print(",");
          client.print ("\"Temp6\":");
          client.print(Temp6);
          client.print(",");
          client.print ("\"Hum6\":");
          client.print(Hum6);
          client.print(",");
          client.print ("\"windSpeed\":");
          client.print(avWindspeed);
          client.print(",");
          client.print ("\"windDir\":");
          client.print(quadrant);
          client.print(",");
          client.print ("\"windGust\":");
          client.print(wspeed);
          client.print(",");
          client.print ("\"windGust24\":");
          client.print(MaxGust24);
          client.print(",");
          client.print ("\"Total Rain\":");
          client.print(rainTotal);
          client.print(",");
          client.print ("\"Rain Rate\":");
          client.print(rainRate);
          client.print(",");
          client.print ("\"outHum\":");
          client.print(outHum);
          client.print("}}}");
          client.println();
          break;
        }


        else if (strstr(clientline, "GET /") != 0) {



          (strstr(clientline, " HTTP"))[0] = 0;
//          Serial.print ("Trick ");
//          Serial.println (clientline);
          char *filename;
          filename = clientline + 5;
          (strstr(clientline, " HTTP"))[0] = 0;

          if (strstr(clientline, "CSV")) {
           // Serial.println (filename);
            if (filename != "favicon.ico")
              client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/plain");
            client.println();
            int16_t c;
            File datalog = SD.open (filename);
            if (datalog) {
              int16_t c;
              while ((c = datalog.read()) > 0) {
                client.print((char)c);
              }
              datalog.close();
            }
            break;
          }
          else if (strstr(clientline, "favicon.ico")) {

            client.stop();
            break;
          }

          else if (strstr(clientline, "time")) {



            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();

            UpdateRTC();
            //Reset RTC to TIME provided
            client.println("<html><body><h2>RTC Reset</h2>");
            tmElements_t tm;
            RTC.read(tm);
            formatTimeDigits(LogMonth, tm.Month);
            formatTimeDigits(LogDay, tm.Day);
            formatTimeDigits(LogHour, tm.Hour);
            formatTimeDigits(LogMinute, tm.Minute);
            formatTimeDigits(LogSecond, tm.Second);
            client.print(LogDay);
            client.print("/");
            client.print(LogMonth);
            client.print("/");
            client.print(year());
            client.print(" ");
            client.print(LogHour);
            client.print(":");
            client.print(LogMinute);
            client.print(":");
            client.print(LogSecond);
            client.println("</body></html>");
            client.stop();
            break;

          }


          else  if (strstr(clientline, "log"))  {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();
            client.println ("Liste des fichiers:");
            root = SD.open("/");
            root.rewindDirectory();
            client.println("<ul>");
            while (true) {
              File entry =  root.openNextFile();
              if (! entry) {
                break;
              }

              client.print("<li><a href=\"");
              client.print("/");
              client.print(entry.name());
              Serial.println(entry.name());
              client.print("\">");
              client.print(entry.name());
              client.print("</a>");
              client.print(' ');

            }
            client.println("</ul>");

            root.close();
            client.stop();
            break;
          }

          else {
            client.println("HTTP/1.1 404 Not Found");
            client.println("Content-Type: text/html");
            client.println();
            client.println("<h2>File Not Found!</h2>");
            client.stop();

          }



        }


      }

    }

    // delay(100);
    // close the connection:
    client.stop();
 
  }
}


