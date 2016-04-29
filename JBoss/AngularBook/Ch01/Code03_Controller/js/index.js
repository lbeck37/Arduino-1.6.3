var HelloCtrl = function ($scope) {
    $scope.name = 'World';

    $scope.getName = function() {
        return $scope.name;
    };
}
