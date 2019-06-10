<?php
include "basicEnum.php";

class mo_options_enum_identity_provider extends BasicEnum{
    const IDP_Base_Url='miniorange_saml_idp_issuer';
}

class mo_options_enum_service_provider extends BasicEnum{
    const Service_Provider_Name ='miniorange_saml_idp_sp_name';
    const ACS_URL = 'miniorange_saml_idp_acs_url';
    const Issuer = 'miniorange_saml_idp_sp_entity_id';
    const NameId_Format = 'miniorange_saml_idp_nameid_format';
    const Relay_State = 'miniorange_saml_idp_relay_state';
    const Assertion_Signed = 'miniorange_saml_idp_assertion_signed';
}