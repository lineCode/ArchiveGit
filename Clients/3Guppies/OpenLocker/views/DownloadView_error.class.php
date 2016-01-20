<?php

class DownloadView extends View
{
    function execute(&$controller, &$request, &$user)
    {
        $renderer =& $request->getAttribute('SmartyRenderer');
        $renderer->setTemplateDir($controller->getModuleDir() . 'templates/');
        $ct = $controller->getContentType();
        switch($ct)
        {
        case 'html':
        case 'wml':
        case 'xhtmlMobile':
            break;
        default:
            $ct = 'html';
            break;
        }
        $renderer->setTemplate("DownloadView_error.$ct.tpl");

        $downloadError = $request->getError('OpenLocker.Download');
        $renderer->setAttribute('Error', $downloadError);

        /*****************************
         *  Log the download attempt *
         ****************************/
        $BrowserInfo =& $user->getAttribute('BrowserInfo');
        $deviceId = 0;
        if( is_object($BrowserInfo) && $BrowserInfo->isValid() )
        {
            $deviceId = $BrowserInfo->getDeviceId();
        }

        $data = $request->getAttribute('OpenLocker.Download.Data');
        if( !is_array($data) )
        {
            $data = array();
        }

        $sendEventId = 0;
        if( array_key_exists('SendEventId', $data) )
        {
            $sendEventId = $data['SendEventId'];
        }

        $sourceType = SOURCE_TYPE_UNKNOWN;
        if(array_key_exists('SourceType', $data))
        {
            $sourceType = $data['SourceType'];
        }

        $toPhoneNumber = 0;
        $recipientIsMember = 0;
        $sentToSelf = null;
        if( array_key_exists('PhoneNumber', $data) )
        {
            $toPhoneNumber = $data['PhoneNumber'];
            $PhoneNumber =& new PhoneNumber(null, $request->getAttribute('read_db'));
            $PhoneNumber->Number($toPhoneNumber);
            $recipientIsMember = $PhoneNumber->isRegistered() ? 1 : 0;

            if( array_key_exists('FromPhoneNumber', $data) )
            {

                $fromPhoneNumber = $data['FromPhoneNumber'];
                if($toPhoneNumber == $fromPhoneNumber)
                {
                    $sentToSelf = 'Self';
                }
                else
                {
                    $sentToSelf = 'Other';
                }
            }
        }

        $mediaClass = 'Personal';
        $mediaId = null;
        if( array_key_exists('MediaId', $data) )
        {
            $mediaId = $data['MediaId'];
            if(!$mediaId)
            {
                $mediaId = null;
            }
        }

        $personalMediaId = null;
        if( array_key_exists('PersonalId', $data) )
        {
            $personalMediaId = $data['PersonalId'];
            if(!$personalMediaId)
            {
                $personalMediaId = null;
            }
        }

        $mediaGenericType = 'Other';
        if( array_key_exists('MediaType', $data) )
        {
            $mediaGenericType = $data['MediaType'];
            switch($mediaGenericType)
            {
            case "Pic":
            case "Picture":
                $mediaGenericType = "Picture";
                break;
            case "Video":
                break;
            case "Audio":
            case "Ringtone":
                $mediaGenericType = "Audio";
                break;
            default:
                $mediaGenericType = "Other";
                break;
            }
        }

        $carrierId = null;
        if( array_key_exists('CarrierId', $data) )
        {
            $carrierId = $data['CarrierId'];
        }
        else
        {
            $carrierId = ThirdPartyCarrierMappings::getCarrierId($toPhoneNumber);
        }

        $mediaFileExtension = null;
        if( array_key_exists('Extension', $data) )
        {
            $mediaFileExtension = $data['Extension'];
        }

        $Event =& new MobilizeDownloadEvent(null, Storage::getMessagingDb($request));
        $Event->Id(0);
        $Event->SendEventId($sendEventId);
        $Event->Date(date("Y-m-d H:i:s"));
        $Event->Status(0);
        $Event->Error($downloadError);
        $Event->SourceType($sourceType);
        $Event->RecipientPhone($toPhoneNumber);
        $Event->RecipientCarrier($carrierId);
        $Event->RecipientIsMember($recipientIsMember);
        $Event->DeviceId($deviceId);
        $Event->SentToSelf($sentToSelf);
        $Event->MediaId($mediaId);
        $Event->PersonalMediaId($personalMediaId);
        $Event->MediaClass($mediaClass);
        $Event->MediaGenericType($mediaGenericType);
        $Event->MediaFileExtension($mediaFileExtension);
        $Event->Attempt(1);

        if($Event->AddOrCommit())
        {

        }
        else
        {

        }

        return $renderer;
    }

}

?>
