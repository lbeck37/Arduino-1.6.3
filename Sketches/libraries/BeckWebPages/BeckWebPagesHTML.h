// BeckWebPagesHTML.h, 5/6/2019a
#pragma once

const char* acThermoWebPageHTML= R"(
<!-- BeckWebPagesHTML.h, 5/6/19c -->
<!DOCTYPE HTML>
<html ng-app="ThermoApp">
<script src="https://ajax.googleapis.com/ajax/libs/angularjs/1.6.9/angular.min.js"></script>
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
  <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.8.1/css/all.css" integrity="sha384-50oBUHEmvpQ+1lW4y57PTFmhCaXp0ML5d60M1M7uH2+nqUivzIebhndOJK28anvf" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body ng-controller="MainCtrl as ctrl">
  <div>
   <h2>Thermostat 5/6/19c</h2>
   <form ng-submit="ctrl.submit() ">
     <p>
        <i class="fas fa-thermometer-half" style="color:#059e8a;"></i>
        <span class="dht-labels">Current</span>
        <span id="LastDegF">{{ctrl.oThermo.dLastDegF}}</span>
        <sup class="units">&deg;F</sup>
      </p>
      <p>
        <i class="fas fa-tachometer-alt" style="color: Tomato;"></i>
        <span class="dht-labels">Setpoint</span>
        <span id="SetPointDegF">{{ctrl.oThermo.dSetpointF}}</span>
        <sup class="units">&deg;F</sup>
      </p>
      <p>
        <i class="fas fa-stroopwafel fa-spin" style="color: Dodgerblue;"></i>
        <span class="dht-labels">Max Heat Range</span>
        <span id="TermoOffDegF">{{ctrl.oThermo.dMaxHeatRangeF}}</span>
        <sup class="units">&deg;F</sup>
      </p>
      <p>
        <i class="fas fa-stroopwafel fa-spin" style="color: Dodgerblue;"></i>
        <span class="dht-labels">Offpoint</span>
        <span id="TermoOffDegF">{{ctrl.dThermoOffDegF}}</span>
        <sup class="units">&deg;F</sup>
      </p>
      <p>
      <input type="submit" value="New Values">
      </p>
    </form>
  </div>

  <script type="text/javascript">
    angular.module('ThermoApp', []).
      controller('MainCtrl', ['$http',
        function($http){
          var self= this;

          self.oThermo= {};
          self.oThermo.dLastDegF;
          self.oThermo.dSetpointF;
          self.oThermo.dMaxHeatRangeF;

          self.dThermoOffDegF;

//***Function declarations begin***
          self.DoThermoGet= function(){
            console.log('DoThermoGet(): Begin');
            ReturnVal= $http.get('/ThermoGet').
              then(function(response){
                self.oThermo= response.data;
                self.dThermoOffDegF= self.oThermo.dSetpointF + self.oThermo.dMaxHeatRangeF;
                console.log('DoThermoGet(): self.oThermo= ', self.oThermo);
              },
              function(errResponse){
                console.error('Error doing $http.get(/LastDegF)');
              });
            return ReturnVal;
          }; //DoThermoGet


          self.DoThermoPost= function(){
            console.log('DoThermoPost(): Begin');
            $http.post('/ThermoPost', self.oThermo).then(self.DoThermoGet)
          };  //DoThermoPost


          self.submit= function(){
            console.log('submit(): Begin');
            self.DoThermoPost();
            self.DoThermoGet();
          };  //submit
//***Function declarations end***

          self.DoThermoGet();
          return;
        } //function($http)
    ]);
</script>
    <!-- Bootstrap required JavaScript -->
    <!-- jQuery first, then Popper.js, then Bootstrap JS -->
    <script src="https://code.jquery.com/jquery-3.3.1.slim.min.js" integrity="sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo" crossorigin="anonymous"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js" integrity="sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1" crossorigin="anonymous"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js" integrity="sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM" crossorigin="anonymous"></script>
</body>
</html>
)";


//Yahoo User Interface Example
const char* acYUI_Dial_InteractiveScrollPicturePageHTML= R"(
<!DOCTYPE HTML>
<html>
<script src="http://yui.yahooapis.com/3.18.1/build/yui/yui-min.js"></script>

<style>
    #example_container {
        position:relative;
    }
    #demo{
        margin:0;
        position:absolute;
        top:321px;
        left:0;
    }
    .controls {
        position:absolute;
        top:0;
        left:328px;
        margin:0 0 0 0;
        color:#808080;
        width:300px;
    }
    .controls a {
        color:#4B78D9 !important;
        cursor:pointer;
    }
    .intro-sentence{
        font-size: 183%;
        left: 0;
        line-height: 0.9em;
        position: absolute;
        top: 273px;
        width: 6em;
    }
    #view_frame{
        position:relative;
        height:500px;
        width:300px;
        border:solid 1px #cccccc;
        overflow:hidden;
    }
    #scene{position:absolute;
        left:0;
        top:-6440px;
        height:6440px;
        width:100%;
        background:url(../assets/dial/images/earth_to_hubble_bkg.png) repeat;
    }
    #altitude_mark {
        border-top:solid 1px #CCCCCC;
        left:-33px;
        position:absolute;
        top:403px;
        width:30px;
    }
    #earth{
        position:absolute;
        left:0;
        top:5834px;
        height:214px;
        width:300px;
    }
    #hubble{
        position:absolute;
        left:5px;
        top:7px;
        height:393px;
        width:300px;
    }
    #stars{
        position:absolute;
        left:0;
        top:0;
        background:url(../assets/dial/images/stars.png) repeat;
        height:5000px;
        width:300px;
    }
    .label{
        text-transform:uppercase;
        width:100%;
        letter-spacing:5px;
        font-family:Verdana;
        font-size:85%;
        position:absolute;
        left:0;
        text-align:center;
    }
    .hubble{
        bottom:6023px;
        color:#612C88;
    }
    .thermosphere{
        bottom:1290px;
        color:#5A009D;
    }
    .mesosphere{
        bottom:840px;
        color:#570BFF;
    }
    .stratosphere{
        bottom:540px;
        color:#006999;
    }
    .troposphere{
        bottom:477px;
        color:#036585;
    }
    .ozone{
        bottom:692px;
        color:#005AAE;
    }
    .crust{
        bottom:270px;
        color:#4F2D00;
    }
    .mantle{
        bottom:42px;
        color:#897701;
    }

</style>

<body class="yui3-skin-sam"> <!-- You need this skin class -->

<div id="example_container">
    <div id="view_frame">
        <div id="scene">
            <div id="stars"></div>
            <img id="hubble" src="http://yuilibrary.com/yui/docs/assets/dial/images/hubble.png"/>
            <img id="earth" src="http://yuilibrary.com/yui/docs/assets/dial/images/mountain_earth.png"/>
            <div class="label hubble">hubble</div>
            <div class="label thermosphere">thermosphere</div>
            <div class="label mesosphere">mesosphere</div>
            <div class="label stratosphere">stratosphere</div>
            <div class="label troposphere">troposphere</div>
            <div class="label ozone">ozone</div>
            <div class="label crust">crust</div>
            <div class="label mantle">mantle</div>
        </div>
    </div>
    <div class="controls">
        <div class="intro-sentence">From Earth to <a id="a-hubble">Hubble</a></div>
        <div id="altitude_mark"></div>
        <div id="demo"></div>
    </div>
</div>

</body>

<script>
YUI().use('dial', function(Y) {

    var sceneH = Y.one('#scene').get('region').height,
    subSea = 450,
    viewFrameH = Y.one('#view_frame').get('region').height -2,
    zeroPt = 100,
    originY = -sceneH + subSea + viewFrameH - zeroPt;

    Y.one('#scene').setStyle('top', originY + 'px');

    /**
    * The Dial's valueChange event is passed to this.
    * sets the CSS top value of the pictoral scene of the earth to the hubble.
    * This scene is an absolute positioned div inside another div with
    * overflow set to hidden.
    */
    setSceneY = function(e) {
        Y.one('#scene').setStyle('top', (originY + (e.newVal * 10)) + 'px');
    }

    var dial = new Y.Dial({
        min:-35,
        max:559,
        stepsPerRevolution:30,
        value: 0,
        diameter: 100,
        minorStep: 1,
        majorStep: 10,
        decimalPlaces: 2,
        strings:{label:'Altitude in Kilometers:', resetStr: 'Reset', tooltipHandle: 'Drag to set'},
        // construction-time event subscription
        after : {
            valueChange: Y.bind( setSceneY, dial )
        }
    });
    dial.render('#demo');

    // Function that calls a method in Dial that sets its value to the value of the max config attribute
    // Other methods available include,
    // _setToMin(), _resetDial(), _incrMinor(), _decrMinor(), _incrMajor(), _decrMajor(),
    var setDialToMax = function(e){
        e.preventDefault();
        this._setToMax();
    }

    // Subscribe to the click of the "Hubble" anchor, passing the dial as the 'this'
    Y.on('click', setDialToMax, '#a-hubble', dial);

});

</script>
</html>
)";

//Last line.
