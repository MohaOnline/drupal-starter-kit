<script>
    jQuery(document).ready(function () {
        getBackupCodes();
    });
</script>
<div id="backupCodeSection">
    <div id="edit-reset" class="form-item form-type-item">
        <div class="resetCode" id="resetCode" onclick="resetBackupCodes()">Reset Code</div>
    </div>
   
    <div id="resettable" class="" style="display: none;">
        <p>The two factor authentication backup code is already generated, please reset your two factor authentication backup code. </p>
    </div>
    
    <div id="lr_ciam_reset_table" style="display: none;">
        <h3>If you lose your phone or can't receive codes via SMS, voice call or Google Authenticator, you can use backup codes to sign in. So please save these backup codes somewhere.</h3>
        <div class="form-item form-type-item">
            <div class="copyMessage" style="display:none;">Copied!</div>
            <div title="Copy" class="mybackupcopy" onclick="changeIconColor()"></div>
        </div>
        <div id="backupcode-table-body"></div>
    </div>   
</div>



