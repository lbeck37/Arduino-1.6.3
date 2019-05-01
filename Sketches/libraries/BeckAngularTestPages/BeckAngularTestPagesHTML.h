// BeckAngularTestPagesHTML.h, 5/1/2019a
#pragma once

const char* acAngularTestPagesHTML= R"(
<!-- BeckAjsTestPage050119.HTML  -->
<!DOCTYPE HTML><html>
<!doctype html>
<html ng-app="ThermoApp">
<head><title>Thermo App</title></head>
<body ng-controller="MainCtrl as ctrl">
  <h1>Thermostat 5/1/2019q</h1>
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


const char* acAngularTestPagesHTML_10= R"(
<!-- BeckAjsTestPage043019.HTML  -->
<!DOCTYPE HTML><html>
<!doctype html>
<html ng-app="ThermoApp">
<head><title>Thermo App</title></head>
<body ng-controller="MainCtrl as ctrl">
  <h1>Thermostat 5/1/2019g</h1>
  <form ng-submit="ctrl.submit() ">
    <div>Current Temperature= {{ctrl.DummyTemp}} </div>
    <div>
      <input type="text" ng-model="ctrl.Thermo.Setpoint">
      New Setpoint will be: {{ctrl.DummyTemp}} Degrees F
    </div>
    <input type="submit" value="DoIt">
  </form>

<script
  src="https://ajax.googleapis.com/ajax/libs/angularjs/1.3.11/angular.js">
</script>
<script type="text/javascript">
  angular.module('ThermoApp', []).
    controller('MainCtrl', ['$http', 
      function($http){
        var self= this;
        var DummyTemp= 37.73;
<!--
        self.Thermo.DegF     = 99.99;
        self.Thermo.Setpoint = 99.99;
        self.Thermo.Offpoint = 99.99;
        var DoThermoGet= function(){
        console.log('DoThermoGet(): Begin', self.Thermo);
          return $http.get('/ThermoGet').then(function(response){
            console.log('Return from $http.get(/ThermoGet) #1, response= ', response);
            self.Thermo.DegF = response.data;
          }, function(errResponse){
            console.error('Error doing $http.get(/ThermoGet) #1');
          }); }
  
       DoThermoGet();
  
       self.DoThermoPost = function(){
        console.log('DoThermoPost(): Begin', self.Thermo);
        $http.post('/ThermoPost', self.Thermo).
          then( DoThermoGet() ).
          then( function(response){self.Thermo= response.data;} );
          };
-->
       var DoThermoGet= function(){
          console.log('DoThermoGet(): Begin', self.Thermo);
          DummyTemp= 47.73;
       };
  
       DoThermoGet();
  
       self.DoThermoPost= function(){
          console.log('DoThermoPost(): Begin', self.Thermo);
          DoThermoGet();
       };
  
       self.submit= function() {
          console.log('User clicked DoIt with ', self.Thermo);
          DoThermoGet();
        };

    }]);
</script>
</body>
</html>
)";


const char* acAngularTestPagesHTML9= R"(
<!-- BeckAjsTestPage043019.HTML, 04/30/19b  -->
<!DOCTYPE HTML><html>
<!doctype html>
<html ng-app="notesApp">
<head><title>Notes App</title></head>
<body ng-controller="MainCtrl as ctrl">

  <form ng-submit="ctrl.submit() ">
    <input type="text" ng-model="ctrl.DegF">
    You typed {{ctrl.DegF}}

    <input type="submit" value="DoIt">
  </form>

<script
  src="https://ajax.googleapis.com/ajax/libs/angularjs/1.3.11/angular.js">
</script>
<script type="text/javascript">
  angular.module('notesApp', [])
    .controller('MainCtrl', ['$http', function($http) {
      var self= this;

      $http.get('/LastDegF').then(function(response) {
        console.log('Return from $http.get() #1, response= ', response);
        self.DegF = response.data;
      }, function(errResponse) {
        console.error('Error while fetching DegF');
      });

      self.submit = function() {
        console.log('User clicked DoIt with ', self.DegF);

        $http.get('/LastDegF').then(function(response) {
          console.log('Return from $http.get) #2, response= ', response);
          self.DegF = response.data;
        }, function(errResponse) {
          console.error('Error while fetching notes');
        });

      };

    }]);
</script>
</body>
</html>
)";


const char* acAngularTestPagesHTML8= R"(
<!doctype html>
<!-- Beck 4/30/19b
From C:\Dev\_Repos\Arduino\Books\AngularJS_UpAndRunning_Book\chapter6\public\http-post-example.html
-->
<html ng-app="BeckApp">

<head>
  <title>Beck $http.get() and post() Example</title>
  <style>
    .item {
      padding: 10px;
    }
  </style>
</head>

<body ng-controller="MainCtrl as mainCtrl">
  <h1>BIOTA 4/30/19c</h1>
    <div>Current DegF= <span ng-bind="mainCtrl.Thermo.DegF"></span></div>
    <div>Setpoint    = <span ng-bind="mainCtrl.Thermo.Setpoint"></span></div>

  <div>
    <form name="ThermoForm"
          ng-submit="mainCtrl.add() ">
  <div>
      <input type="text"
             placeholder="Current DegF"
             ng-model="mainCtrl.Thermo.DegF"
             required>
  </div>
  <div>
      <input type="text"
             placeholder="Set Point"
             ng-model="mainCtrl.Thermo.SetPoint"
             required>
  </div>
  <div>
     <input type="submit"
             value="Go For It"
             ng-disabled="ThermoForm.$invalid">
  </div>
    </form>
  </div>

<script
  src="https://ajax.googleapis.com/ajax/libs/angularjs/1.3.11/angular.js">
</script>
<script>
  angular.module('notesApp', [])
    .controller('MainCtrl', ['$http', function($http) {
      var self              = this;
      self.Thermo           = {};
      self.Thermo.Setpoint  = 71.37;
      var fFetchThermo = function() {
        return $http.get('/LastDegF').then(
          function(response) {
            console.log('Received $http.get(/'LastDegF') response');
            self.Thermo.DegF= response.data;
        }, function(errResponse) {
          console.error('Error while fetching LastDegF');
        });
      };

      fFetchThermo();

      self.add = function() {
<!--
        $http.post('/ajs/post', self.Thermo)
            .then(fFetchThermo)
            .then(function(response) {
              self.Thermo = {};
            });
->
      };

    }]);
</script>
</body>
</html>
)";


const char* acAngularTestPagesHTML7= R"(
<!doctype html>
<html ng-app="GoogleFinance">
  <head>
    <link href="http://twitter.github.io/bootstrap/assets/css/bootstrap.css" rel="stylesheet">
      <script src="http://ajax.googleapis.com/ajax/libs/angularjs/1.0.6/angular.min.js"></script>
     <script src="http://code.angularjs.org/1.0.6/angular-resource.js"></script>
     <script>
      angular.module('GoogleFinance', ['ngResource']);
      function AppCtrl($scope, $resource) {
        $scope.googleFinance = $resource('https://finance.google.com/finance/info', 
                                           {client:'ig', q: 'AAPL', callback:'JSON_CALLBACK'},
                                           {get: {method:'JSONP', isArray: true}});
          $scope.indexResult = $scope.googleFinance.get();
      
        $scope.doSearch = function () {
          console.log($scope.searchTerm)
              $scope.indexResult = $scope.googleFinance.get({q: $scope.searchTerm});
          };
      }
  
     </script>
  </head>
<body>
<div class="container">
  <div class="row">
    <div class="span3">&nbsp;</div>
  </div>  
  <div class="row">
    <div class="span3">
      <div ng-controller="AppCtrl">

  <div class="input-append">
    <input class="span2" type="text" ng-model="searchTerm" />
    <span class="add-on"><i class="icon-search"></i></span>
    <button class="btn" ng-click="doSearch() ">Search</button>
  </div>
  
  Current Price: {{indexResult[0].l_cur}}<br/>
  Change:        {{indexResult[0].c}}<br/>
      </div>
    </div>
  </div>
</div>
</body>
</html>
)";


const char* acAngularTestPagesHTML5= R"(
<!DOCTYPE HTML><html>
<!doctype html>
<!-- File: chapter4/form-error-messages.html -->
<html ng-app="notesApp">
<head><title>Notes App</title></head>
<body ng-controller="MainCtrl as ctrl">

  <form ng-submit="ctrl.submit() " name="myForm">
    <input type="text"
           name="uname"
           ng-model="ctrl.user.username"
           required
           ng-minlength="4">
    <span ng-show="myForm.uname.$error.required">
      This is a required field
    </span>
    <span ng-show="myForm.uname.$error.minlength">
      Minimum length required is 4
    </span>
    <span ng-show="myForm.uname.$invalid">
      This field is invalid
    </span>
    <input type="password"
           name="pwd"
           ng-model="ctrl.user.password"
           required>
    <span ng-show="myForm.pwd.$error.required">
      This is a required field
    </span>
    <input type="submit"
           value="Submit"
           ng-disabled="myForm.$invalid">
  </form>

<script
  src="https://ajax.googleapis.com/ajax/libs/angularjs/1.3.11/angular.js">
</script>
<script type="text/javascript">
  angular.module('notesApp', [])
    .controller('MainCtrl', [function () {
      var self = this;
      self.submit = function () {
        console.log('User clicked submit with ', self.user);
      };
    }]);
</script>

</body>
</html>
)";


const char* acAngularTestPagesHTML4= R"(
<!DOCTYPE HTML><html>
<!doctype html>
<!-- File: chapter4/two-forms-databinding.html -->
<html ng-app="notesApp">
<head><title>Notes App</title></head>
<body ng-controller="MainCtrl as ctrl">

  <form ng-submit="ctrl.submit1() ">
    <input type="text" ng-model="ctrl.username">
    <input type="password" ng-model="ctrl.password">
    <input type="submit" value="Submit">
  </form>

  <form ng-submit="ctrl.submit2() ">
    <input type="text" ng-model="ctrl.user.username">
    <input type="password" ng-model="ctrl.user.password">
    <input type="submit" value="Submit">
  </form>

<script
  src="https://ajax.googleapis.com/ajax/libs/angularjs/1.3.11/angular.js">
</script>
<script type="text/javascript">
  angular.module('notesApp', [])
    .controller('MainCtrl', [function() {
      var self = this;
      self.submit1 = function() {
        // Create user object to send to the server
        var user = {username: self.username, password: self.password};
        console.log('First form submit with ', user);
      };
      self.submit2 = function() {
        console.log('Second form submit with ', self.user);
      };
    }]);
</script>
</body>
</html>
)";


const char* acAngularTestPagesHTML3= R"(
<!DOCTYPE HTML><html>
<!doctype html>
<!-- File: chapter4/simple-form.html -->
<html ng-app="notesApp">
  <head><title>Notes App</title></head>
  <script
    src="https://ajax.googleapis.com/ajax/libs/angularjs/1.3.11/angular.js">
  </script>
  <body ng-controller="MainCtrl as ctrl">

    <form ng-submit="ctrl.submit() ">
      <input type="text" ng-model="ctrl.user.username">
      <input type="password" ng-model="ctrl.user.password">
      <input type="submit" value="Submit">
    </form>

  <script type="text/javascript">
    angular.module('notesApp', [])
      .controller('MainCtrl', [function() {
        var self = this;
        self.submit = function() {
          console.log('User clicked submit with ', self.user);
        };
      }]);
  </script>
  </body>
</html>
)";


const char* acAngularTestPagesHTML2= R"(
<!DOCTYPE HTML><html>
<!doctype html>
<!-- File: chapter4/simple-ng-model.html -->
<html ng-app="notesApp">
<head><title>Notes App</title></head>
<body ng-controller="MainCtrl as ctrl">

  <input type="text" ng-model="ctrl.username"/>
  You typed {{ctrl.username}}

<script
  src="https://ajax.googleapis.com/ajax/libs/angularjs/1.3.11/angular.js">
</script>
<script type="text/javascript">
  angular.module('BeckApp', [])
    .controller('MainCtrl', [function() {
      this.username = 'nothing';
    }]);
</script>
</body>
</html>
 )";


const char* acThermostatTestPagesHTML= R"(
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
      <span id="LastDegF">{{DegF}}</span>
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
