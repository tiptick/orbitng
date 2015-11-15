Pebble.addEventListener('ready', function() {
  console.log('PebbleKit JS ready!');
});

Pebble.addEventListener('showConfiguration', function() {
  var url = 'https://rawgit.com/tiptick/slate-config-example/dev/config/index.html';
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
    ;
//    dict['KEY_COLOR_RED'] = parseInt(backgroundColor.substring(2, 4), 16);
 //   dict['KEY_COLOR_GREEN'] = parseInt(backgroundColor.substring(4, 6), 16);
 //   dict['KEY_COLOR_BLUE'] = parseInt(backgroundColor.substring(6), 16);
  }
  
//if (configData['btvib'] === true){
  dict['PERS_BT_VIB'] = configData['btvib'];
//}
  
//if (configData['backgroundColor'] === true){
  
  var bgc = configData['backgroundColor'];
  dict['PERS_BACK_COLOR'] = parseInt(bgc);
//}
//if (configData['earthColor'] === true){
  dict['PERS_EARTH_COLOR'] = parseInt(configData['earthColor']);
  
//}
//if (configData['moonColor'] === true){
  dict['PERS_MOON_COLOR'] = parseInt(configData['moonColor']);
//}
//if (configData['earthFontBold'] === true){
  dict['PERS_EARTH_FONT_BOLD'] = configData['earthFontBold'];
//}
//if (configData['earthFontInvert'] === true){
  dict['PERS_EARTH_FONT_INVERT'] = configData['earthFontInvert'];
//}
//if (configData['moonFontInvert'] === true){
  dict['PERS_MOON_FONT_INVERT'] = configData['moonFontInvert'];
//} 
    
  

  
  
  // Send to watchapp
  Pebble.sendAppMessage(dict, function() {
    console.log('Send successful: ' + JSON.stringify(dict));
  }, function() {
    console.log('Send failed!');
  });
});