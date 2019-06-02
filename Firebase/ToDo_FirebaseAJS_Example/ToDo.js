// Beck 5/30/19a

var todoApp = angular.module('TodoApp', ['firebase']);

var firebaseConfig = {
    apiKey: "AIzaSyBVOUnaaCjuoFWLsiyVi7ZSW_PsiHLam1A",
    authDomain: "fir-mariestep1.firebaseapp.com",
    databaseURL: "https://fir-mariestep1.firebaseio.com",
    projectId: "fir-mariestep1",
    storageBucket: "fir-mariestep1.appspot.com",
    messagingSenderId: "754022099819",
    appId: "1:754022099819:web:50b456b2ed48aa27"
  };

// Initialize your Firebase app
firebase.initializeApp(firebaseConfig);

todoApp.controller('TodoCtrl', ['$scope', '$firebaseArray', '$firebaseObject',
    function($scope, $firebaseArray, $firebaseObject) {
    // Reference to entire Firebase database
    var todosRef = firebase.database().ref();

        // GET TODOS AS AN ARRAY
        $scope.todos = $firebaseArray(todosRef);

        // ADD_TODO ITEM METHOD
        $scope.addTodo = function () {
            // CREATE A UNIQUE ID
            var timestamp = new Date().valueOf();
            $scope.todos.$add({
                id: timestamp,
                name: $scope.todoName,
                status: 'pending'
            });
            $scope.todoName = "";
        };  //addTodo


        // REMOVE_TODO ITEM METHOD
        $scope.removeTodo = function (index, todo) {
            // CHECK THAT ITEM IS VALID
            if (todo.id === undefined) return;

            // FIREBASE: REMOVE ITEM FROM LIST
            $scope.todos.$remove(todo);
        };  //removeTodo


        // MARK TODO AS IN PROGRESS METHOD
        $scope.startTodo = function (index, todo) {
            // CHECK THAT ITEM IS VALID
            if (todo.id === undefined)return;

            // UPDATE STATUS TO IN PROGRESS AND SAVE
            todo.status = 'in progress';
            $scope.todos.$save(todo);
        };  //startTodo


        // MARK TODO AS COMPLETE METHOD
        $scope.completeTodo = function (index, todo) {
            // CHECK THAT ITEM IS VALID
            if (todo.id === undefined)return;

            // UPDATE STATUS TO COMPLETE AND SAVE
            todo.status = 'complete';
            $scope.todos.$save(todo);
        };  //completeTodo
    }]);  //controller
//Last Line.