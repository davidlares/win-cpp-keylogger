Param([String]$Att,[String]$Subject,[String]$Body)
Function SendEmail(){
    Param(
        [Parameter(Mandatory=$true)][String]$To,
        [Parameter(Mandatory=$true)][String]$From,
        [Parameter(Mandatory=$true)][String]$Password,
        [Parameter(Mandatory=$true)][String]$Subject,
        [Parameter(Mandatory=$true)][String]$Body,
        [Parameter(Mandatory=$true)][String]$attachment
    )

    try {
       $Msg = New-Object System.Net.Mail.MailMessage($From,$To,$subject,$Body);
       $Srv = "smtp.gmail.com";
       if($attachment -ne $null){
        try {
            $Attachments = $attachment -split ("\:\:");
            ForEach($val in $Attachments) {
                $attach = New-Object System.Net.Mail.Attachment($val);
                $Msg.Attachments.Add($attach);
            }
        } catch {
            exit 2;
        }
        $client = New-Object System.Net.Mail.SmtpCLient($Srv, 587);
        $client.EnableSsl = true;
        $client.Credentials = New-Object System.Net.NetworkCredential($From.Split("@")[0]);
        $client.Send($Msg);
        Remove-Variable -Name Client
        Remove-Variable -Name Password
        exit 7;   
       }
    } catch {
        exit 3;
    }
}

try {
    SendEmail 
        -attachment $Att 
        -To "new@example.com" 
        -Body $body 
        -Subject $Subject 
        -password "secure-password"
        -From "Sender Address"
} catch {
    exit 4;
}