addOnPostRun(function() {
  try {
    // Crash the program
    _cFunc();
  }
  catch(e) {
    // Catch the abort
    out(true);
  }
  out("again");
  try {
    // Try executing a function directly
    _cFunc();
    out("never");
  }
  catch(e) {
    out(e);
  }
  out("ccall");
  try {
    // Try executing a function via ccall
    Module.ccall("cFunc");
    out("never");
  }
  catch(e) {
    out(e);
  }
  out("cwrap");
  try {
    // Try executing a function via cwrap
    Module.cwrap("cFunc")();
    out("never");
  }
  catch(e) {
    out(e);
  }
  out("embind");
  try {
    // Try executing an embind function
    Module.embindFunc();
    out("never");
  }
  catch(e) {
    out(e);
  }
})
