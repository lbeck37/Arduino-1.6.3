//BeckAsyncWebServerHTML.h, 4/27/19a
#pragma once

const char* acAngularTestPagesHTML= R"(
<!DOCTYPE HTML><html>
<!doctype html>
<!-- File: chapter5/item-service-using-service/index.html -->
<html ng-app="notesApp">
<body ng-controller="MainCtrl as mainCtrl">
  <h1>Hello Controllers!</h1>
  <button ng-click="mainCtrl.open('first') ">Open First</button>
  <button ng-click="mainCtrl.open('second') ">Open Second</button>
  <div ng-switch on="mainCtrl.tab">

    <div ng-switch-when="first">
      <div ng-controller="SubCtrl as ctrl">
        <h3>First tab</h3>
        <ul>
          <li ng-repeat="item in ctrl.list() ">
            <span ng-bind="item.label"></span>
          </li>
        </ul>

        <button ng-click="ctrl.add() ">Add More Items</button>
      </div>

    </div>
    <div ng-switch-when="second">
      <div ng-controller="SubCtrl as ctrl">
        <h3>Second tab</h3>
        <ul>
          <li ng-repeat="item in ctrl.list() ">
            <span ng-bind="item.label"></span>
          </li>
        </ul>

        <button ng-click="ctrl.add() ">Add More Items</button>
      </div>
    </div>
  </div>

  <script src="https://ajax.googleapis.com/ajax/libs/angularjs/1.3.11/angular.js"></script>
  <script src="app.js"></script>

<script>
// File: chapter5/item-service-using-service/app.js

function ItemService() {
  var items = [
    {id: 1, label: 'Item 0'},
    {id: 2, label: 'Item 1'}
  ];
  this.list = function() {
    return items;
  };
  this.add = function(item) {
    items.push(item);
  };
}

angular.module('notesApp', [])
  .controller('MainCtrl', [function() {
    var self = this;
    self.tab = 'first';
    self.open = function(tab) {
      self.tab = tab;
    };
  }])
  .controller('SubCtrl',
      ['ItemService', function(ItemService) {
    var self = this;
    self.list = function() {
      return ItemService.list();
    };

    self.add = function() {
      ItemService.add({
        id: self.list().length + 1,
        label: 'Item ' + self.list().length
      });
    };
  }])
  .service('ItemService', [ItemService]);
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
