// BeckAngularTestPagesHTML.h, 4/30/19a
#pragma once

const char* acAngularTestPagesHTML= R"(
<!doctype html>
<!-- Beck 4/30/19a
From C:\Dev\_Repos\Arduino\Books\AngularJS_UpAndRunning_Book\chapter6\public\http-post-example.html
-->
<html ng-app="notesApp">

<head>
  <title>HTTP Post Example</title>
  <style>
    .item {
      padding: 10px;
    }
  </style>
</head>

<body ng-controller="MainCtrl as mainCtrl">
  <h1>Hello Servers!</h1>
  <div ng-repeat="todo in mainCtrl.items"
       class="item">
    <div><span ng-bind="todo.label"></span></div>
    <div>- by <span ng-bind="todo.author"></span></div>
  </div>

  <div>
    <form name="addForm"
          ng-submit="mainCtrl.add() ">
      <input type="text"
             placeholder="Label"
             ng-model="mainCtrl.newTodo.label"
             required>
      <input type="text"
             placeholder="Author"
             ng-model="mainCtrl.newTodo.author"
             required>
      <input type="submit"
             value="Add"
             ng-disabled="addForm.$invalid">
    </form>
  </div>

<script
  src="https://ajax.googleapis.com/ajax/libs/angularjs/1.3.11/angular.js">
</script>
<script>
  angular.module('notesApp', [])
    .controller('MainCtrl', ['$http', function($http) {
      var self = this;
      self.items = [];
      self.newTodo = {};
      var fetchTodos = function() {
        return $http.get('/ajs/get').then(
            function(response) {
          self.items = response.data;
        }, function(errResponse) {
          console.error('Error while fetching notes');
        });
      };

<!--
      fetchTodos();
-->
      self.add = function() {
<!--
        $http.post('/ajs/post', self.newTodo)
            .then(fetchTodos)
            .then(function(response) {
              self.newTodo = {};
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


const char* acAngularTestPagesHTML6= R"(
<!DOCTYPE HTML><html>
<!doctype html>
<!-- File: chapter4/simple-form.html -->
<html ng-app="notesApp">
<head><title>Notes App</title></head>
<body ng-controller="MainCtrl as ctrl">

  <form ng-submit="ctrl.submit() ">
    <input type="text" ng-model="ctrl.Current.DegF">
    You typed {{ctrl.Current.DegF}}

    <input type="submit" value="DoIt">
  </form>

<script
  src="https://ajax.googleapis.com/ajax/libs/angularjs/1.3.11/angular.js">
</script>
<script type="text/javascript">
  angular.module('notesApp', [])
    .controller('MainCtrl', ['$http', function($http) {
      var Biota = this;

      $http.get('/LastDegF').then(function(response) {
        console.log('Return from $http.get(), response= ', response);
        Biota.DegF = response.data;
      }, function(errResponse) {
        console.error('Error while fetching notes');
      });

      Biota.submit = function() {
        console.log('User clicked DoIt with ', Biota.Current);

        $http.get('/LastDegF').then(function(response) {
          console.log('Return from $http.get), response= ', response);
          Biota.DegF = response.data;
        }, function(errResponse) {
          console.error('Error while fetching notes');
        });

      };

    }]);
</script>
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
  angular.module('notesApp', [])
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
