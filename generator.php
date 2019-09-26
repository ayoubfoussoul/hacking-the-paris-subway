<?php

set_time_limit(0);


#Auxiliary functions
function station_data($stationName, $stationLine, $maxA, $maxR, $maxPer){

    $request = '
    <soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:xsd="http://wsiv.ratp.fr/xsd" xmlns:wsiv="http://wsiv.ratp.fr">
    <soapenv:Header/>
    <soapenv:Body>
    <wsiv:getMissionsNext>
    <wsiv:station>
    <xsd:line>
    <xsd:id>' . $stationLine . '</xsd:id>
    </xsd:line>
    <xsd:name>' . $stationName . '</xsd:name>
    </wsiv:station>
    <wsiv:direction>
    <xsd:sens>*</xsd:sens>
    </wsiv:direction>
    </wsiv:getMissionsNext>
    </soapenv:Body>
    </soapenv:Envelope>';


    $location = "http://opendata-tr.ratp.fr/wsiv/services/Wsiv?wsdl=";
    $uri      = "http://opendata-tr.ratp.fr/wsiv/services";
    $action   = "urn:getMissionsNext";
    $version  = 0;

    $client    = new SoapClient(null, array('location' => $location, 'uri' => ""));
    $xmlstring = $client->__doRequest($request, $location, $action, $version);
    $clean_xml = str_ireplace(['SOAPENV:', 'NS1:', 'NS2:'], '', $xmlstring);
    $xml       = simplexml_load_string($clean_xml);

    #echo htmlspecialchars($clean_xml);

    $return        = $xml->Body->getMissionsNextResponse->return;
    $perturbations = array();
    $missions      = array();

    $resP1A = '';
    $resP1R = '';
    $numMissionsA = 0;
    $numMissionsR = 0;
    $resP2 = '';
    $numPerturbations = 0;

    foreach($return->missions as $mission) {
        if($mission->direction->sens == "A" and $numMissionsA <= $maxA) {
            $numMissionsA++;
            $resP1A = $resP1A . ',' . $mission->stationsDates . ',"' . $mission->stationsMessages .'"';
        }
        else if($numMissionsR <= $maxR){
            $numMissionsR++;
            $resP1R = $resP1R . ',' . $mission->stationsDates . ',"' . $mission->stationsMessages .'"';
        }
    }
    for($i = 0; $i < $maxA-$numMissionsA; $i++) $resP1A = $resP1A . ',,""';
    for($i = 0; $i < $maxR-$numMissionsR; $i++) $resP1R = $resP1R . ',,""';
    #if ($numMissionsA == 0) $resP1A = ',';
    #if ($numMissionsR == 0) $resP1R = ',';

    foreach($return->perturbations as $perturbation) {
        if($numPerturbations <= $maxPer){
        $resP2 = $resP2 . ',"' .$perturbation->message->text.'"';
        $numPerturbations++;
        }
    }
    for($i = 0; $i < $maxPer-$numPerturbations; $i++) $resP2 = $resP2 . ',""';

    return $return->argumentDate . ',"' . $stationName .'",'
            . $numMissionsA 
            . $resP1A . ',' 
            . $numMissionsR 
            . $resP1R . ',' . $numPerturbations . $resP2;

}

function getStations($line){
    
    $request = '
    <soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:xsd="http://wsiv.ratp.fr/xsd" xmlns:wsiv="http://wsiv.ratp.fr">
    <soapenv:Header/>
     <soapenv:Body>
     <wsiv:getStations>
        <wsiv:station>
            <xsd:line>
                <xsd:id>'. $line .'</xsd:id>
            </xsd:line>
        </wsiv:station>
     </wsiv:getStations>
     </soapenv:Body>
    </soapenv:Envelope>
    ';


    $location = "http://opendata-tr.ratp.fr/wsiv/services/Wsiv?wsdl=";
    $uri      = "http://opendata-tr.ratp.fr/wsiv/services";
    $action   = "urn:getStations";
    $version  = 0;

    $client = new SoapClient(null, array('location' => $location,
                                        'uri'       => ""));
    $xmlstring = $client->__doRequest($request, $location, $action, $version);
    $clean_xml = str_ireplace(['SOAPENV:', 'NS1:', 'NS2:'], '', $xmlstring);
    $xml       = simplexml_load_string($clean_xml);

    $return = $xml->Body->getStationsResponse->return;

    $res = array();

    foreach($return->stations as $station){
        array_push($res, $station->name);
    }
    return $res;

}

function addData($file, $stations, $line, $maxA, $maxR, $maxPer){
    
    foreach ($stations as $station) {
        $fp = fopen($file, 'a');
        fwrite($fp, station_data($station, $line, $maxA, $maxR, $maxPer) . "\n");
        fclose($fp);
        sleep(1/150);
    }

}

function addHeader($file, $maxA, $maxR, $maxPer){
    $fp = fopen($file, 'w');

    $header = '"time","station name","number of departures",';
    for($i = 1 ; $i < $maxA + 1; $i++) $header = $header . '"departure number ' . $i . '",' . '"description departure number ' . $i . '",';
    $header = $header . '"number of arrivals",';
    for($i = 1 ; $i < $maxR + 1; $i++) $header = $header . '"arrival number ' . $i . '",'  . '"description arrival number ' . $i . '",';
    $header = $header . '"number of perturbations",';
    for($i = 1 ; $i < $maxPer; $i++) $header = $header . '"perturbation number ' . $i . '",';
    $header = $header . '"perturbation number ' . $maxPer . '"' . "\n";

    fwrite($fp, $header);
    fclose($fp);

}


#Global variables
$maxA = 8;
$maxR = 8;
$maxPer = 4;
$line = "RB";
$file = 'RER_B_data.csv';
$timeInMinutes = 720; #12H


#Main loop
$stations = getStations($line);
$time = 0;
addHeader($file, $maxA, $maxR, $maxPer);
while($time < $timeInMinutes){
    $time++;
    addData($file, $stations, $line, $maxA, $maxR, $maxPer);
    sleep(60);
}
