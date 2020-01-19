// Beck 1/19/20a
//*
var todoApp = angular.module('TodoApp', ['firebase']);


todoApp.controller('TodoCtrl', ['$scope', '$firebaseArray',
    function($scope, $firebaseArray) {

        // CREATE A FIREBASE REFERENCE
        //var todosRef = new Firebase('https://your-firebase-url.firebaseio.com/');
        var todosRef = new Firebase('https://fir-mariestep1.firebaseio.com/');

        // GET TODOS AS AN ARRAY
        $scope.todos = $firebaseArray(todosRef);

        // ADD TODO ITEM METHOD
        $scope.addTodo = function () {

            // CREATE A UNIQUE ID
            var timestamp = new Date().valueOf();

            $scope.todos.$add({
                id: timestamp,
                name: $scope.todoName,
                status: 'pending'
            });

            $scope.todoName = "";

        };

        // REMOVE TODO ITEM METHOD
        $scope.removeTodo = function (index, todo) {

            // CHECK THAT ITEM IS VALID
            if (todo.id === undefined)return;

            // FIREBASE: REMOVE ITEM FROM LIST
            $scope.todos.$remove(todo);

        };

        // MARK TODO AS IN PROGRESS METHOD
        $scope.startTodo = function (index, todo) {

            // CHECK THAT ITEM IS VALID
            if (todo.id === undefined)return;

            // UPDATE STATUS TO IN PROGRESS AND SAVE
            todo.status = 'in progress';
            $scope.todos.$save(todo);

        };

        // MARK TODO AS COMPLETE METHOD
        $scope.completeTodo = function (index, todo) {

            // CHECK THAT ITEM IS VALID
            if (todo.id === undefined)return;

            // UPDATE STATUS TO COMPLETE AND SAVE
            todo.status = 'complete';
            $scope.todos.$save(todo);
        };

    }]);
    //*/
    var app = angular.module("sampleApp", ["firebase"]);

	// a factory to create a re-usable Profile object
	// we pass in a username and get back their synchronized data as an object
	app.factory("Profile", ["$firebaseObject",
	  function($firebaseObject) {
	    return function(username) {
	      // create a reference to the database node where we will store our data
	      var ref = firebase.database().ref("rooms").push();
	      var profileRef = ref.child(username);

	      // return it as a synchronized object
	      return $firebaseObject(profileRef);
	    }
	  }
	]);

	app.controller("ProfileCtrl", ["$scope", "Profile",
	  function($scope, Profile) {

        // CREATE A FIREBASE REFERENCE
        //var todosRef = new Firebase('https://your-firebase-url.firebaseio.com/');
        var todosRef = new Firebase('https://fir-mariestep1.firebaseio.com/');

        // GET TODOS AS AN ARRAY
        $scope.todos = $firebaseArray(todosRef);

	    // create a three-way binding to our Profile as $scope.profile
	    Profile("physicsmarie").$bindTo($scope, "profile");
	  }
	]);
//Last line.