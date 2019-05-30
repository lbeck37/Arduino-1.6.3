// Thermostat.js, 5/29/19b
    angular.module('ThermoApp', []).
      controller('MainCtrl', ["$scope", "$firebaseObject",
        function($scope, $firebaseObject){
      //controller('MainCtrl', ["$scope",
        //function($scope){
      //controller('MainCtrl', [
        //function(){
          var self= this;

          self.oThermo= {};
          self.oThermo.dLastDegF      = 70.37;
          self.oThermo.dSetpointF     = 71.00;
          self.oThermo.dMaxHeatRangeF =  0.10;

          self.dThermoOffDegF= self.oThermo.dSetpointF + self.oThermo.dMaxHeatRangeF;

          self.submit= function(){
            console.log('submit(): Begin');
          };  //submit

          return;
        } //function()
    ]);
//Last line.
