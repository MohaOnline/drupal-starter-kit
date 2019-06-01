//jQuery(".user_pricing").on("change", function() {	
function loadLicensing() {

	document.getElementsByClassName("user_pricing")[0].onchange = function() {	
       //var userNumber = jQuery(this).val();
	   var userNumber = document.getElementsByClassName("user_pricing")[0].options[document.getElementsByClassName("user_pricing")[0].selectedIndex].value;
	 //  alert(userNumber);
     if(userNumber == "custom"){
			document.getElementById("mo_idp_enter_user").style.display = 'block'; 
			document.getElementById("custom_users_price").style.display = 'block';
			// jQuery("#mo_idp_enter_user").show();
			// jQuery("#custom_users_price").show();
        } else{
			document.getElementById("mo_idp_enter_user").style.display = 'none';
			document.getElementById("custom_users_price").style.display = 'none';
            //jQuery("#mo_idp_enter_user").hide();
			//jQuery("#custom_users_price").hide();
        } }; 
	document.getElementsByClassName("user_price")[0].onchange = function() {
       //var userNumber = jQuery(this).val();
	   //alert("Here1");
	   var userNumber = document.getElementsByClassName("user_price")[0].options[document.getElementsByClassName("user_price")[0].selectedIndex].value;
       if(userNumber == "custom"){
		   document.getElementById("mo_idp_enter_user_premium").style.display = 'block'; 
		   document.getElementById("custom_users_premium_price").style.display = 'block'; 
			//jQuery("#mo_idp_enter_user_premium").show();
			//jQuery("#custom_users_premium_price").show();
        }else{
			document.getElementById("mo_idp_enter_user_premium").style.display = 'none';
			document.getElementById("custom_users_premium_price").style.display = 'none';
            //jQuery("#mo_idp_enter_user_premium").hide();
			//jQuery("#custom_users_premium_price").hide();
        }
    };
    document.getElementsByClassName("custom_users_premium")[0].onblur = function() {
        //var no_of_users = jQuery(this).val();
		var no_of_users = document.getElementsByClassName("custom_users_premium")[0].value;
        var isnum = /^\d+$/.test(no_of_users);
        if(isnum && no_of_users > 0){
            var user_price = calculateManualUserPrice(no_of_users);
			var p=user_price.toString();
			var price='$'.concat(p);
			//jQuery("#custom_users_premium_price").val(user_price);	
			document.getElementById("custom_users_premium_price").value = price;
        }
    };
	document.getElementsByClassName("custom_users")[0].onblur = function() {
        //var no_of_users = jQuery(this).val();
		var no_of_users = document.getElementsByClassName("custom_users")[0].value;
        var isnum = /^\d+$/.test(no_of_users);
        if(isnum && no_of_users > 0){
            var user_price = calculateManualUserPrice(no_of_users);
			var p=user_price.toString();
			var price='$'.concat(p);
			document.getElementById("custom_users_price").value = price;
			//jQuery("#custom_users_price").val(user_price);	
        }
    };
	
    function calculateManualUserPrice(userNumber){
        userPriceMap = new Array();
        userPriceMap["5"] = 15;
        userPriceMap["10"] = 30;
        userPriceMap["20"] = 45;
        userPriceMap["30"] = 60;
        userPriceMap["40"] = 75;
        userPriceMap["50"] = 90;
        userPriceMap["60"] = 100;
        userPriceMap["70"] = 110;
        userPriceMap["80"] = 120;
        userPriceMap["90"] = 130;
        userPriceMap["100"] = 140;
        userPriceMap["150"] = 177.5;
        userPriceMap["200"] = 215;  
        userPriceMap["250"] = 245;  
        userPriceMap["300"] = 275;  
        userPriceMap["350"] = 300;  
        userPriceMap["400"] = 325;  
        userPriceMap["450"] = 347.5;
        userPriceMap["500"] = 370;  
        userPriceMap["600"] = 395;  
        userPriceMap["700"] = 420;  
        userPriceMap["800"] = 445;  
        userPriceMap["900"] = 470;  
        userPriceMap["1000"] = 495; 
        userPriceMap["2000"] = 549; 
        userPriceMap["3000"] = 599; 
        userPriceMap["4000"] = 649; 
        userPriceMap["5000"] = 699; 
        userPriceMap["10000"] = 799;
        userPriceMap["20000"] = 999;
        if(userPriceMap[userNumber]){
                return userPriceMap[userNumber];
        }else{
            if(userNumber < 10){
                return userNumber * 3;
            }else if(userNumber > 10 && userNumber < 50){
                return 30 + (userNumber - 10) * 1.5;
            }else if(userNumber > 50 && userNumber < 100){
                return 90 + (userNumber - 50) * 1;
            }else if(userNumber > 100 && userNumber < 200){
                return 140 + (userNumber - 100) * 0.75;
            }else if(userNumber > 200 && userNumber < 300){
                return 215 + (userNumber - 200) * 0.60;
            }else if(userNumber > 300 && userNumber < 400){
                return 275 + (userNumber - 300) * 0.50;
            }else if(userNumber > 400 && userNumber < 500){
                return 325 + (userNumber - 400) * 0.45;
            }else if(userNumber > 500 && userNumber < 1000){
                return 370 + (userNumber - 500) * 0.25;
            }else if(userNumber > 1000 && userNumber < 2000){
                return 495 + (userNumber - 1000) * 0.054;
            }else if(userNumber > 2000 && userNumber < 5000){
                return 549 + (userNumber - 2000) * 0.05;
            }else if(userNumber > 5000 && userNumber < 20000){
                return 699 + (userNumber - 5000) * 0.02;
            }else{
                return 1499;
            }
        }
    }
	
}

