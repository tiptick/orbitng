Pebble.addEventListener('ready', function() {
  console.log('PebbleKit JS ready!');
});

Pebble.addEventListener('showConfiguration', function() {
  var url = 'https://rawgit.com/tiptick/slate-config-example/master/config/index.html';
  console.log('Showing configuration page: ' + url);

  Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
  var configData = JSON.parse(decodeURIComponent(e.response));
  console.log('Configuration page returned: ' + JSON.stringify(configData));

//  var backgroundColor = configData['background_color'];

  var dict = {};
  if(configData['date'] === true) {
    dict['PERS_DATE'] = configData['date'];
  } else {
//    dict['KEY_COLOR_RED'] = parseInt(backgroundColor.substring(2, 4), 16);
 //   dict['KEY_COLOR_GREEN'] = parseInt(backgroundColor.substring(4, 6), 16);
 //   dict['KEY_COLOR_BLUE'] = parseInt(backgroundColor.substring(6), 16);
  }

  // Send to watchapp
  Pebble.sendAppMessage(dict, function() {
    console.log('Send successful: ' + JSON.stringify(dict));
  }, function() {
    console.log('Send failed!');
  });
});