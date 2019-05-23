// Beck 5/21/19

var firebaseConfig = {
apiKey: "AIzaSyBVOUnaaCjuoFWLsiyVi7ZSW_PsiHLam1A",
authDomain: "fir-mariestep1.firebaseapp.com",
databaseURL: "https://fir-mariestep1.firebaseio.com",
projectId: "fir-mariestep1",
storageBucket: "fir-mariestep1.appspot.com",
messagingSenderId: "754022099819",
appId: "1:754022099819:web:50b456b2ed48aa27"
};
// Initialize Firebase
firebase.initializeApp(firebaseConfig);


// Reference to the recommendations object in your Firebase database
var recommendations = firebase.database().ref("recommendations");

// Save a new recommendation to the database, using the input in the form
var submitRecommendation = function () {

  // Get input values from each of the form elements
  var title = $("#talkTitle").val();
  var presenter = $("#talkPresenter").val();
  var link = $("#talkLink").val();

  // Push a new recommendation to the database using those values
  recommendations.push({
    "title": title,
    "presenter": presenter,
    "link": link
  });
};

// When the window is fully loaded, call this function.
// Note: because we are attaching an event listener to a particular HTML element
// in this function, we can't do that until the HTML element in question has
// been loaded. Otherwise, we're attaching our listener to nothing, and no code
// will run when the submit button is clicked.
$(window).load(function () {

  // Find the HTML element with the id recommendationForm, and when the submit
  // event is triggered on that element, call submitRecommendation.
  $("#recommendationForm").submit(submitRecommendation);

});