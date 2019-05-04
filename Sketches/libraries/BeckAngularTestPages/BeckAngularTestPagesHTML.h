// BeckAngularTestPagesHTML.h, 5/2/2019a
#pragma once

const char* acThermostatTestPagesHTML= R"(
<!-- BeckThermostatTestPagesHTML.h, 5/3/19c -->
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
   <h2>BIOTA 5/3/19ab</h2>
   <form ng-submit="ctrl.submit() ">
     <p>
        <i class="fas fa-thermometer-half" style="color:#059e8a;"></i>
        <span class="dht-labels">Current</span>
        <span id="LastDegF">{{ctrl.oThermo.dDegF}}</span>
        <sup class="units">&deg;F</sup>
      </p>
      <p>
        <i class="fas fa-tachometer-alt" style="color: Tomato;"></i>
        <span class="dht-labels">Setpoint</span>
        <span id="SetPointDegF">{{ctrl.oThermo.dSetpoint}}</span>
        <sup class="units">&deg;F</sup>
      </p>
      <p>
        <i class="fas fa-stroopwafel fa-spin" style="color: Dodgerblue;"></i>
        <span class="dht-labels">Max Heat Range</span>
        <span id="TermoOffDegF">{{ctrl.oThermo.dMaxHeatRange}}</span>
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
          self.oThermo.dDegF    = 99.99;
          self.oThermo.dSetpoint= 88.99;
          self.oThermo.dMaxHeatRange= 77.99;

       
          self.DoThermoGet= function(){
            console.log('DoThermoGet(): Begin');
            ReturnVal= $http.get('/ThermoGet').
              then(function(response){
/*
                console.log('Return from $http.get(/ThermoGet), response= ', response);
                console.log('DoThermoGet(): self.oThermo(1)= ', self.oThermo);
                console.log('DoThermoGet(): response.data= ', response.data);
                console.log('DoThermoGet(): response.data.dDegF= ', response.data.dDegF);
*/
                self.oThermo= response.data;
                console.log('DoThermoGet(): self.oThermo= ', self.oThermo);
/*
                console.log('DoThermoGet(): self.oThermo.dDegF= '    , self.oThermo.dDegF);
                console.log('DoThermoGet(): self.oThermo.dSetpoint= ', self.oThermo.dSetpoint);
                console.log('DoThermoGet(): self.oThermo.dMaxHeatRange= ', self.oThermo.dMaxHeatRange);
                console.log('DoThermoGet(): self.oThermo(2)= ', self.oThermo);
*/
              }, 
              function(errResponse){
                console.error('Error doing $http.get(/LastDegF)');
              });
            return ReturnVal;
          }; //DoThermoGet

            self.DoThermoGet();
            
            self.DoThermoPost= function(){
              console.log('DoThermoPost(): Begin');
              $http.post('/ThermoPost', self.oThermo).then(self.DoThermoGet)
            };  //DoThermoPost
            
            self.submit= function(){
              console.log('submit(): Begin');
              self.DoThermoPost();
            };  //submit        
        } //function($http)
    ]);
</script>
</body>
</html>
)";


const char* acAngularTestPagesHTML= R"(
<!-- BeckAjsTestPage050119.HTML  -->
<!DOCTYPE HTML><html>
<!doctype html>
<html ng-app="ThermoApp">
<head><title>Thermo App</title></head>
<body ng-controller="MainCtrl as ctrl">
  <h1>Thermostat 5/1/2019r</h1>
  <form ng-submit="ctrl.submit() ">
    <div>Current Temperature= {{ctrl.DummyTemp}} </div>
    <div>
      <input type="text" ng-model="ctrl.Thermo.Setpoint">
      New Setpoint will be: {{ctrl.DummyTemp}} Degrees F
    </div>
    <input type="submit" value="DoIt">
  </form>

<script src="https://ajax.googleapis.com/ajax/libs/angularjs/1.3.11/angular.js"> </script>
<script type="text/javascript">
  angular.module('ThermoApp', []).
    controller('MainCtrl', ['$http', 
      function($http){
          var self= this;
          self.DummyTemp;

          self.DoThermoGet= function(){
            console.log('DoThermoGet(): Begin');
            self.DummyTemp= 47.73;
          };  //DoThermoGet
  
          self.DoThermoGet();
          
          self.DoThermoPost= function(){
            console.log('DoThermoPost(): Begin');
            self.DoThermoGet();
          };  //DoThermoPost
          
          self.submit= function(){
            console.log('submit(): Begin');
            self.DoThermoPost();
          };  //submit        
      } //function($http)
  ]);
</script>
</body>
</html>
)";


const char* acThermostatTestPagesHTML_1= R"(
<!-- BeckThermostatTestPagesHTML.h, 4/26/19a -->
<!DOCTYPE HTML>
<html>
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
<body>
  <div ng-app="" ng-init= "DegF='70.37'; SetPoint='72.00'; OffPoint='72.10'">
    <h2>BIOTA</h2>
    <p>
      <i class="fas fa-thermometer-half" style="color:#059e8a;"></i>
      <span class="dht-labels">Current</span>
      <span id="LastDegF">{{dDegF}}</span>                     
      <sup class="units">&deg;F</sup>
    </p>
    <p>
      <i class="fas fa-tachometer-alt" style="color: Tomato;"></i>
      <span class="dht-labels">Setpoint</span>
      <span id="SetPointDegF">{{SetPoint}}</span>
      <sup class="units">&deg;F</sup>
    </p>
    <p>
      <i class="fas fa-stroopwafel fa-spin" style="color: Dodgerblue;"></i>
      <span class="dht-labels">Offpoint</span>
      <span id="TermoOffDegF">{{OffPoint}}</span>
      <sup class="units">&deg;F</sup>
    </p>
  </div>
</body>
</html>
)";
//Last line.
