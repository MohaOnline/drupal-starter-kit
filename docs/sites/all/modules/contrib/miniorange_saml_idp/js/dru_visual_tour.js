var moTour;
var pointerNumber = 0;
var nextTabOn =false;
var lastTab =false;
(function($)
{
    Drupal.behaviors.moTour = {
        attach: function (context, settings) {
            moTour = settings.moTour;
            tryHere(settings.moTour);
        }
    };
})(jQuery);

function tryHere(moTour)
{
    jQuery("#edit-miniorange-saml-idp-support-side-button").click(function (e) {
        e.preventDefault();
        if (jQuery("#mosaml-feedback-form").css("right") != "0px") {
            jQuery("#mosaml-feedback-overlay").show();
            jQuery("#mosaml-feedback-form").animate({
                "right": "0px"
            });
        }
        else {
            jQuery("#mosaml-feedback-overlay").hide();
            jQuery("#mosaml-feedback-form").animate({
                "right": "-391px"
            });
        }
    });


    $mo =jQuery;
    addIDs();
    $mo("#mo_saml_restart_tour_button").insertAfter($mo(".otp-header").find(".mo-otp-help-button"));
    if(!moTour.tourTaken)
        startTour(pointerNumber);
    $mo("#mo_saml_restart_tour_button").click( function() {
        resetTour();
        startTour(pointerNumber);
    });
}

function startTour(pointerNumber){
    addOverlay();
    createCard(pointerNumber);
}

function addOverlay() {
    var overlay = ' <div class="mo-tour-backdrop" ><div id="overlay" class="mo-tour-overlay"></div></div>';
    jQuery(overlay).insertAfter('div.add-or-remove-shortcuts.add-shortcut');
}
function createCard(pointerNumber)
{
    var tourElement =   moTour.tourData[pointerNumber];
    var card        =   '<div id="mo-card" class="mo-card mo-'+tourElement.cardSize+'">'
        +'  <div class="mo-tour-arrow mo-point-'+tourElement.pointToSide+'"><div style="color:#ffffff;position: relative;" class="mo-saml-arrow mo-saml-arrow-'+tourElement.pointToSide+'"></div></div>'
        +'  <div class="mo-tour-content-area mo-point-'+tourElement.pointToSide+'"><div class="mo-tour-title mo-saml-header">'+tourElement.titleHTML+'</div><div class="mo-tour-content">'+tourElement.contentHTML+'</div><img '+(tourElement.img.visible ? '':'hidden')+' src="'+tourElement.img.src+'" alt=""> '
        +'      <div class="mo-tour-button-area"></div>'
        +'<div hidden class="mo-tour-card-bottom"></div></div></div>';
    var nextButton  =   '<input type="button"  class="mo-tour-button mo-tour-primary-btn" value="'+tourElement.buttonText+'">';
    var skipButton  =   '<input type="button" '+ (tourElement.ifskip? tourElement.ifskip : '') +' class="mo-tour-button mo-skip-btn" value="Skip Tour"> '

    jQuery(card).insertAfter('#overlay');
    if(moTour) jQuery('.mo-tour-button-area').append(skipButton);
    if(tourElement.ifNext) jQuery('.mo-tour-button-area').append(nextButton);

    if(tourElement.pointToSide=='' || tourElement.pointToSide=='center') jQuery('.mo-card').attr('style','box-shadow:0px 0px 20px 7px #979393;');

    jQuery('.mo-target-index').removeClass('mo-target-index');
    if(tourElement.targetE)    getPointerPosition(tourElement.targetE,tourElement.pointToSide);

    jQuery('.mo-tour-primary-btn').click( function(){
        if(moTour.tourData[pointerNumber].action) {
            eval(moTour.tourData[pointerNumber].action);
        }
        setTimeout( function() {
            pointerNumber+=1;
            if(moTour.tourData[pointerNumber]){
                jQuery('.mo-card').remove();
                createCard(pointerNumber);
            }
            else {
                if( !moTour.tourTaken && moTour.tourData['nextTab'] && !nextTabOn) {
                    jQuery('.mo-card').remove();
                    nextTabOn=true;
                    createCard('nextTab');
                }
                else {
                    tourComplete(moTour.pageID);
                    resetTour();
                }
            }
        },500);
    });
    jQuery(".mo-skip-btn").click( function() {
        resetTour();
    });
}
function resetTour() {
    pointerNumber=0;
    nextTabOn=false;
    if(jQuery('.mo-tour-backdrop'))
        jQuery('.mo-tour-backdrop').remove();
    jQuery('.mo-target-index').removeClass('mo-target-index');
}

function addIDs() {
    jQuery(moTour.addID).each( function(){
        if(jQuery(this.selector))
            jQuery(this.selector).attr('id',this.newID);
    });
}

function tourComplete(pageID) {
    jQuery.ajax({
        url: moTour.pageURL,
        type: "POST",
        data: { doneTour : true,pageID:pageID },
        dataType: "json",
    });
}
function getPointerPosition(targetE,pointToSide) {
    document.getElementById(targetE).scrollIntoView({
        behavior: 'auto',
        block: 'center',
        inline: 'center'
    });
    var targetDimentions = document.getElementById(targetE).getBoundingClientRect();
    var cardDimentions   = document.getElementById('mo-card').getBoundingClientRect();
    var finalLeft,finalTop;
    switch(pointToSide) {
        case 'up' :
            finalLeft   =   targetDimentions.left + (targetDimentions.width - cardDimentions.width)/2 ;
            finalTop    =   targetDimentions.top + targetDimentions.height +15;
            break;
        case 'down' :
            finalLeft   =   targetDimentions.left + (targetDimentions.width - cardDimentions.width)/2 ;
            finalTop    =   targetDimentions.top - cardDimentions.height ;
            break;
        case 'left' :
            finalLeft   =   targetDimentions.left + targetDimentions.width;
            finalTop    =   targetDimentions.top + (targetDimentions.height - cardDimentions.height)/2 ;
            break;
        case 'right' :
            finalLeft   =   targetDimentions.left - cardDimentions.width;
            finalTop    =   targetDimentions.top + (targetDimentions.height - cardDimentions.height)/2 ;
            break;
    }

    if(targetE == 'mosaml-feedback-form'){
        finalTop-=130;
        finalLeft-=32;
    }
    jQuery('.mo-card').css({'top':finalTop,'left':finalLeft,'margin-top':'0','margin-left':'0'});
    jQuery('#'+targetE).addClass('mo-target-index');
    if(targetE != 'mosaml-feedback-form'){
        jQuery('#'+targetE).addClass('mo-target-position');
    }
}

