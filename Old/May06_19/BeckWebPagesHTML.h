// BeckWebPagesHTML.h, 5/6/2019a
#pragma once

const char* acThermoWebPageHTML= R"(
<!-- BeckWebPagesHTML.h, 5/6/19c -->
<!DOCTYPE HTML>
<html ng-app="ThermoApp">
<script src="https://ajax.googleapis.com/ajax/libs/angularjs/1.6.9/angular.min.js"></script>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
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
</body>
</html>
)";
//Last line.
