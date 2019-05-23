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


// Initialize your Firebase app
firebase.initializeApp(firebaseConfig);

// Reference to your entire Firebase database
var myFirebase = firebase.database().ref();

// Get a reference to the recommendations object of your Firebase.
// Note: this doesn't exist yet. But when we write to our Firebase using
// this reference, it will create this object for us!
var recommendations = myFirebase.child("recommendations");

// Push our first recommendation to the end of the list and assign it a
// unique ID automatically.
recommendations.push({
    "title": "The danger of a single story",
    "presenter": "Chimamanda Ngozi Adichie",
    "link": "https://www.ted.com/talks/chimamanda_adichie_the_danger_of_a_single_story"
});