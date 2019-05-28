static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0, shrink-to-fit=no">
    <style>

      canvas{
    -moz-user-select: none;
    -webkit-user-select: none;
    -ms-user-select: none;
  }

#avance
{
  height: 110px;
  width: 110px; 
/*  background:url("https://upload.wikimedia.org/wikipedia/commons/thumb/4/4d/Znak_D-3.svg/220px-Znak_D-3.svg.png") no-repeat;  
  background-size: contain; */
  }

#avance:hover,#halte:hover,#recule:hover,#gauche:hover,#droite:hover /* Apparence au survol des liens */
{
   opacity: 0.6;
}

#avance:active,#halte:active,#recule:active,#gauche:active,#droite:active
{
   box-shadow: 10px 10px 10px black; 
}

#halte
{
  height: 120px;
  width:120px; 
/*  background-image:url("https://i.imgur.com/QGzfhXu.gif");
  background-size: contain; */
}

#recule
{
  height: 110px;
  width: 110px; 
/*  background:url("https://i.imgur.com/pDZdveb.png") no-repeat;
  background-size: contain;  */
  }

#gauche
{
  height: 110px;
  width: 110px; 
/*  background:url("https://i.imgur.com/KpzZECB.png") no-repeat;
  background-size: contain;   */
  } 

#droite
{
  height: 110px;
  width: 110px; 
/*  background:url("https://i.imgur.com/gpH5Wvl.png") no-repeat;
  background-size: contain;  */
  }

p
{
    margin-left: auto;
    margin-right: auto;
    width: 100%
}

#1
{
      display: flex;
    flex-direction: row;
    justify-content: center;
    width: 100%;
}
  
#dataTable {
    font-family: "Trebuchet MS", Arial, Helvetica, sans-serif;
    border-collapse: collapse;
    width: 100%;
  }

#dataTable td, #dataTable th {
    border: 1px solid #ddd;
    padding: 8px;
  }

  #dataTable tr:nth-child(even){background-color: #f2f2f2;}

  #dataTable tr:hover {background-color: #ddd;}

  #dataTable th {
    padding-top: 12px;
    padding-bottom: 12px;
    text-align: left;
    background-color: #4CAF50;
    color: white;
  }

  #chart
  {
    display: flex;
    flex-direction: raw;
    justify-content: center;    
    length : 900px;
  }
  div
  {
    display: flex;
    flex-direction: raw;
    justify-content: center;    
    length : 900px;    
  }
 </style> 

<script>
var websock;
function start() {
  websock = new WebSocket('ws://' + window.location.hostname + ':81/');
  websock.onopen = function(evt) { console.log('websock open'); };
  websock.onclose = function(evt) { console.log('websock close'); };
  websock.onerror = function(evt) { console.log(evt); };
/*  websock.onmessage = function(evt) {
    console.log(evt);
    var e = document.getElementById();
    if (evt.data === 'avance') {
      e.style.color = 'red';
    }
    else if (evt.data === 'halte') {
      e.style.color = 'black';
    }
    else {
      console.log('unknown event');   
    } 
  };  */
}
function buttonclick(e) {
  websock.send(e.id);
}
  
    function avance() {
  websock.send("avance");
    }
    

    function halte() {
  websock.send("halte");
    }
   
    function recule() {
  websock.send("recule");
    }

    function gauche() {
  websock.send("gauche");
    }

    function droite() {
  websock.send("droite");
    }
    
</script>
  </head>
<body onload="javascript:start();">
<div >
    <button id="avance" type="button" value="avance" onclick="avance();" /> 
</div>   
<div id="trois">
    <input id="gauche" type="button" value="gauche" onclick="gauche();" />

    <input id="halte" type="button" value="halte" onclick="halte();" />

    <input id="droite" type="button" value="droite" onclick="droite();" />  
</div>
<div>
    <input id="recule" type="button" value="recule" onclick="recule();" />
</div>
</body>
</html>
)rawliteral";
