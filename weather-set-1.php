<?php

date_default_timezone_set('America/New_York'); # Set your timezonr here.

$details = json_decode(file_get_contents("http://ipinfo.io/"));
$ipinfoZip = $details->postal;


if ($_GET["temp"] == 'Y') {
	
	# CALL WITH http://[IPADDRESS]:84/weather-set-1.php?temp=Y
	$current_temp = get_current_temp ($ipinfoZip);
	echo $current_temp . "<br>";	
	$formatted_current_temp = str_ireplace("&deg;", "", $current_temp);
	
	$formatted_current_temp = "T".str_pad($formatted_current_temp, 4, "0", STR_PAD_LEFT)."ZZ"; 
	echo $formatted_current_temp;
	$pass_current_temp = file_get_contents('http://127.0.0.1:83/'.$formatted_current_temp);
	echo $pass_current_temp;

	
} else {
	# CALL WITH http://[IPADDRESS]:84/weather-set-1.php
	$raw_weather_code = get_weather_code ($ipinfoZip); 
	$clean_weather_code = "W".str_pad($raw_weather_code, 3, "0", STR_PAD_LEFT)."WW"; 
	echo $clean_weather_code . "<br/>"; 
	$pass_weather_code = file_get_contents('http://127.0.0.1:83/'.$clean_weather_code);
	echo $pass_weather_code;
}






#Start Functions

function get_contents() {

  file_get_contents("http://" . $_SERVER['SERVER_ADDR'] . ":83/H");
  $output = var_dump($http_response_header);
  return $output;
}


function get_weather_code ($zipcode)
{
# The following code is based on: http://www.hastuts.com/display-weather-forecast-by-zip-code-using-php/
   //yahoo api call
   $api_result = file_get_contents('http://weather.yahooapis.com/forecastrss?p=' . $zipcode . '&u=f');
   $xml_data = simplexml_load_string($api_result);
   //get data
   $xml_data->registerXPathNamespace('yweather', 'http://xml.weather.yahoo.com/ns/rss/1.0');
   //get Weather location
   $location = $xml_data->channel->xpath('yweather:location');
   $output = '';
   if(!empty($location)) {
      foreach($xml_data->channel->item as $item)
      {
         $current = $item->xpath('yweather:condition');
         $forecast = $item->xpath('yweather:forecast');
         $current = $current[0];
         //format data
         $output = "{$current['code']}";
      }
   } else {
         $output = '<h3>No weather forecast found</h3>';
   }
   return $output;
}

function get_current_temp ($zipcode)
{
# The following code is based on: http://www.hastuts.com/display-weather-forecast-by-zip-code-using-php/
   //yahoo api call
   $api_result = file_get_contents('http://weather.yahooapis.com/forecastrss?p=' . $zipcode . '&u=f');
   $xml_data = simplexml_load_string($api_result);
   //get data
   $xml_data->registerXPathNamespace('yweather', 'http://xml.weather.yahoo.com/ns/rss/1.0');
   //get Weather location
   $location = $xml_data->channel->xpath('yweather:location');
   $output = '';
   if(!empty($location)) {
      foreach($xml_data->channel->item as $item)
      {
         $current = $item->xpath('yweather:condition');
         $forecast = $item->xpath('yweather:forecast');
         $current = $current[0];
         //format data
         $output = "{$current['temp']}&deg;F";
      }
   } else {
         $output = '<h3>No weather forecast found</h3>';
   }
   return $output;
}

function show_weather ($zipcode)
{
   # The following code is based on: http://www.hastuts.com/display-weather-forecast-by-zip-code-using-php/
   //yahoo api call
   $api_result = file_get_contents('http://weather.yahooapis.com/forecastrss?p=' . $zipcode . '&u=f');
   $xml_data = simplexml_load_string($api_result);
   //get data
   $xml_data->registerXPathNamespace('yweather', 'http://xml.weather.yahoo.com/ns/rss/1.0');
   //get Weather location
   $location = $xml_data->channel->xpath('yweather:location');
   $output = '';
   if(!empty($location)) {
      foreach($xml_data->channel->item as $item)
      {
         $current = $item->xpath('yweather:condition');
         $forecast = $item->xpath('yweather:forecast');
         $current = $current[0];
         //format data
         $output = '<h2>Weather forecast</h2><br />';
         $output .= 'Time: ' . date('g:i A') .'<br />';
         //get weather image based on temperature
         $output .= "
         <h3>Location: {$location[0]['city']}, {$location[0]['region']}</h3>
         <span style=\"font-size:50px; font-weight:bold;\">{$current['temp']}&deg;F</span>
         <br />
		 {$current['code']}
		 <br/>
         <img src=\"http://l.yimg.com/a/i/us/we/52/{$current['code']}.gif\" style=\"vertical-align: middle;\"          />&nbsp;
         {$current['text']}<br />
         <u>Future forecast</u><br />
         {$forecast[0]['day']} - {$forecast[0]['text']}. High: {$forecast[0]['high']} Low: {$forecast[0]['low']}
         <br/>
         {$forecast[1]['day']} - {$forecast[1]['text']}. High: {$forecast[1]['high']} Low: {$forecast[1]['low']}";
      }
   } else {
         $output = '<h3>No weather forecast found</h3>';
   }
   return $output;
}

#End Functions
?>
