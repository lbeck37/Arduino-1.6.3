// Thermostat.js, 5/22/19b
// <script type="text/javascript">
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
//</script>
