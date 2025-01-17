//****************
app.controller("MyCtrl", ["$scope", "$firebaseObject",
  function($scope, $firebaseObject) {
     var ref = firebase.database().ref();

     var obj = $firebaseObject(ref);

     // to take an action after the data loads, use the $loaded() promise
     obj.$loaded().then(function() {
        console.log("loaded record:", obj.$id, obj.someOtherKeyInData);

       // To iterate the key/value pairs of the object, use angular.forEach()
       angular.forEach(obj, function(value, key) {
          console.log(key, value);
       });
     });

     // To make the data available in the DOM, assign it to $scope
     $scope.data = obj;

     // For three-way data bindings, bind it to the scope instead
     obj.$bindTo($scope, "data");
  }
]);


//****************
// The key of a root reference is null
var rootRef = firebase.database().ref();
var key = rootRef.key;  // key === null


//****************
// The key of any non-root reference is the last token in the path
var adaRef = firebase.database().ref("users/ada");
var key = adaRef.key;  // key === "ada"
key = adaRef.child("name/last").key;  // key === "last"