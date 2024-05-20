var jsonData = JSON.parse(msg.payload);
msg.payload = jsonData;
return msg;