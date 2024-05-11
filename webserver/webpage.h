const char mainpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8" />
  <meta http-equiv="X-UA-Compatible" content="IE=edge" />
  <link rel="preconnect" href="https://fonts.googleapis.com" />
  <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin />


  <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/js/bootstrap.bundle.min.js"
    integrity="sha384-MrcW6ZMFYlzcLA8Nl+NtUVF0sA7MsXsP1UyJoMp4YLEuNSfAP+JcXn/tWtIaxVXM"
    crossorigin="anonymous"></script>
  <link href="https://fonts.googleapis.com/css2?family=Prompt&display=swap" rel="stylesheet" />

  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <!-- <link rel="stylesheet" href="./css/bootstrap.min.css"> -->
  <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css" rel="stylesheet"
    integrity="sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC" crossorigin="anonymous" />
  <title>Dashboard</title>
</head>

<body style="font-family: 'Prompt', sans-serif">

  <div class="row">
    <div class="col">
      <button type="button" class="btn btn-primary" onclick="downloadlog(GYROlog,'GYRO')">
        Download log
      </button>
    </div>
  </div>

  <script>



    InitWebSocket();




    function InitWebSocket() {

      var websock = new WebSocket('ws://' + window.location.hostname + ':81/');

      websock.onclose = function (e) {
        console.log('Socket is closed. Reconnect will be attempted in 1 second.', e.reason);
        //setTimeout(function() {
        InitWebSocket();
        //  }, 500);
      };
      websock.onerror = function (err) {
        console.error('Socket encountered error: ', err.message, 'Closing socket');
        websock.close();
      };

       websock.onmessage = function (evt) {
        JSONobj = JSON.parse(evt.data);
        var text = JSON.stringify(GYROlog);
        var GYROlog_count = GYROlog.length();

        if (JSONobj.state == 1 || GYROlog_count <= 3000) {
          console.log("update: ", JSONobj.state);
          GYROlog.push([
            JSONobj.state,
            [
              JSONobj.time,
              JSONobj.valueAccX,
              JSONobj.valueAccY,
              JSONobj.valueAccZ,
              JSONobj.valueGyX,
              JSONobj.valueGyY,
              JSONobj.valueGyZ,
            ]
          ]);
        }
        else {
          console.log("Stop");
        }
      }
    }
    var GYROlog = []




    function download(filename, text) {
      var element = document.createElement('a');
      element.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(text));
      element.setAttribute('download', filename);

      element.style.display = 'none';
      document.body.appendChild(element);

      element.click();

      document.body.removeChild(element);
    }

    function downloadlog(hardware, name) {
      download(name + "_log.txt", JSON.stringify(hardware));
    }

  </script>
</body>

</html>
)=====";