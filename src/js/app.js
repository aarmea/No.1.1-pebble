const CONFIGURATION_URL = 'https://aarmea.github.io/No.1.1-pebble/';

function sendOptionsToPebble(options) {
  Pebble.sendAppMessage(options, function() {
    console.log('Options sent to Pebble');
  }, function(e) {
    console.log('Failed to send options to Pebble: ' + e.data.error.message);
  });
}

// Stolen from https://stackoverflow.com/questions/679915/how-do-i-test-for-an-empty-javascript-object
function isObjectEmpty(object) {
  return Object.keys(object).length === 0 && JSON.stringify(object) ===
    JSON.stringify({});
}

Pebble.addEventListener('ready', function() {
  console.log('No.1.1 JS app boot');
  var options = JSON.parse(localStorage.getItem('configuration'));
  if (options === null || isObjectEmpty(options)) {
    console.log('No saved settings found');
    return;
  }

  sendOptionsToPebble(options);
});

Pebble.addEventListener('showConfiguration', function() {
  console.log('Showing configuration');
  Pebble.openURL(CONFIGURATION_URL + '?config=' +
      encodeURIComponent(localStorage.getItem('configuration')));
});

Pebble.addEventListener('webviewclosed', function(e) {
  var options = JSON.parse(decodeURIComponent(e.response));
  if (isObjectEmpty(options)) {
    console.log('Options canceled');
    return;
  }

  var optionsString = JSON.stringify(options);
  localStorage.setItem('configuration', optionsString);
  console.log('Options changed: ' + optionsString);

  sendOptionsToPebble(options);
});
