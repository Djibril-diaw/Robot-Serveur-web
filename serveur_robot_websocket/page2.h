static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0, shrink-to-fit=no">
    <style>

/* gère l'apparence de la page */

.switch {
  position: relative;
  display: inline-block;
  width: 100px;
  height: 34px;
}

.switch input {
  display: none;
}

.slider {
  position: absolute;
  cursor: pointer;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: #f25221;
  -webkit-transition: .4s;
  transition: .4s;
}

.slider:before {
  position: absolute;
  content: "";
  height: 26px;
  width: 26px;
  left: 4px;
  bottom: 4px;
  background-color: white;
  -webkit-transition: .4s;
  transition: .4s;
}

input:checked+.slider {
  background-color: #2196F3;
}

input:focus+.slider {
  box-shadow: 0 0 1px #2196F3;
}

input:checked+.slider:before   {
  -webkit-transform: translateX(66px);
  -ms-transform: translateX(66px);
  transform: translateX(66px);
}

/*------ ADDED CSS ---------*/
.on
{
  display: none;
 
}
.on:after {
  padding-left: 10px;
}

.on, .off
{
  color: white;
  position: absolute;
  transform: translate(-50%,-50%);
  top: 50%;
  left: 50%;
  font-size: 10px;
  font-family: Verdana, sans-serif;
}


input:checked+ .slider .on
{display: block;
}

input:checked + .slider .off
{display: none;}
/*--------- END --------*/


/* Rounded sliders */

.slider.round {
  border-radius: 34px;
}

.slider.round:before {
  border-radius: 50%;
}

#avance
{
  height: 110px;
  width: 110px; 
  background:url("https://upload.wikimedia.org/wikipedia/commons/thumb/4/4d/Znak_D-3.svg/220px-Znak_D-3.svg.png") no-repeat;  
  background-size: contain; 
  }

#avance:hover,#halte:hover,#recule:hover,#gauche:hover,#droite:hover /* Apparence au survol des liens */
{
   opacity: 0.6;
}

#gyro 
{
  height: 80px;
  width: 140px;
  justify-content :right;
}

#avance:active,#halte:active,#recule:active,#gauche:active,#droite:active
{
   box-shadow: 10px 10px 10px black; 
}

#halte
{
  height: 120px;
  width:120px; 
  background-image:url("https://i.imgur.com/QGzfhXu.gif");
  background-size: contain; 
}

#recule
{
  height: 110px;
  width: 110px; 
  background:url("https://i.imgur.com/pDZdveb.png") no-repeat;
  background-size: contain;  
  }

#gauche
{
  height: 110px;
  width: 110px; 
  background:url("https://i.imgur.com/KpzZECB.png") no-repeat;
  background-size: contain;  
  } 

#droite
{
  height: 110px;
  width: 110px; 
  background:url("https://i.imgur.com/gpH5Wvl.png") no-repeat;
  background-size: contain;  
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
var width = (window.innerWidth > 0) ? window.innerWidth : screen.width;
var gyroselect = 0;
var websock;
function start() {
  websock = new WebSocket('ws://' + window.location.hostname + ':81/');
  websock.onopen = function(evt) { console.log('websock open'); };
  websock.onclose = function(evt) { console.log('websock close'); };
  websock.onmessage = function(evt) {console.log(evt);};
  websock.onerror = function(evt) { console.log(evt); };
}

// permet de switch entre le gyroscope et les boutons
function gyroclick() {
  if (gyroselect == 0){
    gyroselect = 1;
  }
  else {
    gyroselect = 0;
  }
  console.log('gyroselect = '+ gyroselect);
}


//fonctions controlant le déplacement du robot
    function avance() {
      if(gyroselect == 0){
  websock.send("1");
    }
  }

    function halte() {
      if(gyroselect == 0){
  websock.send("2");
    }
  }
    function recule() {
      if(gyroselect == 0){ 
  websock.send("3");
    }
  }
    function gauche() {
      if(gyroselect == 0){
  websock.send("4");
    }
  }
    function droite() {
      if(gyroselect == 0){
  websock.send("5");
    }
  }

// les fonctions suivantes utilisent le gyroscope du téléphone
      (function (root, factory) {
    if (typeof define === 'function' && define.amd) {
        // AMD. Register as an anonymous module.
        define(factory);
    } else if (typeof exports === 'object') {
        // Node. Does not work with strict CommonJS, but
        // only CommonJS-like enviroments that support module.exports,
        // like Node.
        module.exports = factory();
    } else {
        // Browser globals (root is window)
        root.gyro = factory();
  }
}(this, function () {
  var measurements = {
        x: null,
        y: null,
        z: null,
        alpha: null,
        beta: null,
        gamma: null
      },
      calibration = {
        x: 0,
        y: 0,
        z: 0,
        alpha: 0,
        beta: 0,
        gamma: 0,
        rawAlpha: 0,
        rawBeta: 0,
        rawGamma: 0
      },
      interval = null,
      features = [];

  var gyro = {};

  /**
   * @public
   */
  gyro.frequency = 200; //ms

  gyro.calibrate = function() {
    for (var i in measurements) {
      calibration[i] = (typeof measurements[i] === 'number') ? measurements[i] : 0;
    }
  };

  gyro.getOrientation = function() {
    return measurements;
  };

  gyro.startTracking = function(callback) {
    interval = setInterval(function() {
      callback(measurements);
    }, gyro.frequency);
  };

  gyro.stopTracking = function() {
    clearInterval(interval);
  };

  /**
   * Current available features are:
   * MozOrientation
   * devicemotion
   * deviceorientation
   */
  gyro.hasFeature = function(feature) {
    for (var i in features) {
      if (feature == features[i]) {
        return true;
      }
    }
    return false;
  };

  gyro.getFeatures = function() {
    return features;
  };

  /**
   * @private
   */
  function eulerToQuaternion(e) {
    var s = Math.PI / 180;
    var x = e.beta * s, y = e.gamma * s, z = e.alpha * s;
    var cX = Math.cos(x / 2);
    var cY = Math.cos(y / 2);
    var cZ = Math.cos(z / 2);
    var sX = Math.sin(x / 2);
    var sY = Math.sin(y / 2);
    var sZ = Math.sin(z / 2);
    var w = cX * cY * cZ - sX * sY * sZ;
    x = sX * cY * cZ - cX * sY * sZ;
    y = cX * sY * cZ + sX * cY * sZ;
    z = cX * cY * sZ + sX * sY * cZ;
    return {x:x, y:y, z:z, w:w};
  }
  gyro.eulerToQuaternion=eulerToQuaternion;

  /**
   * @private
   */
  function quaternionMultiply(a, b) {
    return {
      w: a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
      x: a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
      y: a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
      z: a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w
    };
  }

  /**
   * @private
   */
  function quaternionApply(v, a) {
    v = quaternionMultiply(a, {x:v.x,y:v.y,z:v.z,w:0});
    v = quaternionMultiply(v, {w:a.w, x:-a.x, y:-a.y, z:-a.z});
    return {x:v.x, y:v.y, z:v.z};
  }

  /**
   * @private
   */
  function vectorDot(a, b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
  }

  /**
   * @private
   */
  function quaternionToEuler(q) {
    var s = 180 / Math.PI;
    var front = quaternionApply({x:0,y:1,z:0}, q);
    var alpha = (front.x == 0 && front.y == 0) ?
      0 : -Math.atan2(front.x, front.y);
    var beta = Math.atan2(front.z,Math.sqrt(front.x*front.x+front.y*front.y));
    var zgSide = {
      x: Math.cos(alpha),
      y: Math.sin(alpha),
      z: 0
    };
    var zgUp = {
      x: Math.sin(alpha) * Math.sin(beta),
      y: -Math.cos(alpha) * Math.sin(beta),
      z: Math.cos(beta)
    };
    var up = quaternionApply({x:0,y:0,z:1}, q);
    var gamma = Math.atan2(vectorDot(up, zgSide), vectorDot(up, zgUp));

    // wrap-around the value according to DeviceOrientation
    // Event Specification
    if (alpha < 0) alpha += 2 * Math.PI;
    if (gamma >= Math.PI * 0.5) {
      gamma -= Math.PI; alpha += Math.PI;
      if (beta > 0) beta = Math.PI - beta;
      else beta = -Math.PI - beta;
    } else if (gamma < Math.PI * -0.5) {
      gamma += Math.PI; alpha += Math.PI;
      if (beta > 0) beta = Math.PI - beta;
      else beta = -Math.PI - beta;
    }
    if (alpha >= 2 * Math.PI) alpha -= 2 * Math.PI;
    return {alpha: alpha * s, beta: beta * s, gamma: gamma * s};
  }

  /**
   * @private
   */
  // it doesn't make sense to depend on a "window" module
  // since deviceorientation & devicemotion make just sense in the browser
  // so old school test used.
  if (window && window.addEventListener) {
    function setupListeners() {
      function MozOrientationInitListener (e) {
        features.push('MozOrientation');
        e.target.removeEventListener('MozOrientation', MozOrientationInitListener, true);

        e.target.addEventListener('MozOrientation', function(e) {
          measurements.x = e.x - calibration.x;
          measurements.y = e.y - calibration.y;
          measurements.z = e.z - calibration.z;
        }, true);
      }
      function deviceMotionListener (e) {
        features.push('devicemotion');
        e.target.removeEventListener('devicemotion', deviceMotionListener, true);

        e.target.addEventListener('devicemotion', function(e) {
          measurements.x = e.accelerationIncludingGravity.x - calibration.x;
          measurements.y = e.accelerationIncludingGravity.y - calibration.y;
          measurements.z = e.accelerationIncludingGravity.z - calibration.z;
        }, true);
      }
      function deviceOrientationListener (e) {
        features.push('deviceorientation');
        e.target.removeEventListener('deviceorientation', deviceOrientationListener, true);

        e.target.addEventListener('deviceorientation', function(e) {
          var calib = eulerToQuaternion({
            alpha: calibration.rawAlpha,
            beta: calibration.rawBeta,
            gamma: calibration.rawGamma
          });
          calib.x *= -1; calib.y *= -1; calib.z *= -1;

          var raw = eulerToQuaternion({
            alpha: e.alpha, beta: e.beta, gamma: e.gamma
          });
          var calibrated = quaternionMultiply(calib, raw);
          var calibEuler = quaternionToEuler(calibrated);

          measurements.alpha = calibEuler.alpha;
          measurements.beta = calibEuler.beta;
          measurements.gamma = calibEuler.gamma;

          measurements.rawAlpha = e.alpha;
          measurements.rawBeta = e.beta;
          measurements.rawGamma = e.gamma;
        }, true);
      }

      window.addEventListener('MozOrientation', MozOrientationInitListener, true);
      window.addEventListener('devicemotion', deviceMotionListener, true);
      window.addEventListener('deviceorientation', deviceOrientationListener, true);
    }
    setupListeners();
  }

  return gyro;
}));


    gyro.startTracking(function(o) {
      var b = document.getElementById('example'),
          f = document.getElementById('features');
      f.innerHTML = gyro.getFeatures();
      b.innerHTML = "<p> x = " + o.x + "</p>" +
                    "<p> y = " + o.y + "</p>" +
                    "<p> z = " + o.z + "</p>" +
                    "<p> alpha = " + o.alpha + "</p>" +
                    "<p> beta = " + o.beta + "</p>" +
                    "<p> gamma = " + o.gamma + "</p>";
  if (gyroselect == 1){    
    // permet de controler le déplacment du robot en fonction de l'inclinaison du téléphone              
    if (o.gamma > -40 && o.gamma < 40){
      
      if (o.beta <-10) {
      websock.send("1");
      }
      else if (o.beta <45 && o.beta> -10) {
      websock.send("2");                      
      }
      else if (o.beta > 45){
      websock.send("3");
      }
    }
    else if (o.gamma < -40) {
      websock.send("4");
    }
    else if (o.gamma >40) {
      websock.send("5");
    }
    else {
      console.log('pas de gyro'); 
    }
  }
 });

    
    
</script>
  </head>
<body onload="javascript:start();">

<div >
<label class="switch">
    <input type="checkbox" id="gyro" value="gyro" onchange="gyroclick();" checked/> 
  <div class="slider round">
     <span class="on">buttons</span><span class="off">gyro</span>
  </div>
</label>
</div> 
</br>
<div >
    <button id="avance" type="button" value="avance" onclick="avance();" /> 
</div>   
<div id="trois">
    <button id="gauche" type="button" value="gauche" onclick="gauche();" />

    <button id="halte" type="button" value="halte" onclick="halte();" />

    <button id="droite" type="button" value="droite" onclick="droite();" />  
</div>
<div>
    <input id="recule" type="button" value="recule" onclick="recule();" />
</div>

<p>gyro.getFeatures: <span id="features"></span></p>
  <span id="example"></span>
  <script>
  console.log('width = '+ width);
//if (width <1300) {




  </script>

  
</body>
</html>
)rawliteral";
