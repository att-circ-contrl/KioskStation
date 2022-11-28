////////////////////////////////////////////////////////////////////////////////////////
// Events                                                                             //
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// Constants                                                                          //
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// Variables                                                                          //
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// Constructor & Destructor                                                           //
////////////////////////////////////////////////////////////////////////////////////////
if(document.getElementById('wd_iframe_api') === null){
	var tag = document.createElement('script');
	tag.src = "https://www.youtube.com/iframe_api";
	tag.id = "wd_iframe_api";
	var firstScriptTag = document.getElementsByTagName('script')[0];
	firstScriptTag.parentNode.insertBefore(tag, firstScriptTag);	
}

var wdPlayers = [];
var playerVars_ = {};
var wdTargetPlayers = [];
////////////////////////////////////////////////////////////////////////////////////////
// Public Methods                                                                     //
////////////////////////////////////////////////////////////////////////////////////////
window.onYouTubeIframeAPIReady = function() {
	jQuery("div.ytwd_main_iframe").each(function(index){
		var id = jQuery(this).attr("id");
	    var key = jQuery(this).attr("data-id");
		getPlayerVars(jQuery(this).attr("data-src"));
		var videoID = jQuery(this).attr("data-video-id"); 
		wdPlayers[index] = new YT.Player(id, {
			videoId: videoID,
			width: '100%',			
			playerVars: playerVars_,
			events: {
				'onReady': ytwdOnPlayerReady,
				'onError': ytwdOnPlayerError
			}
		});

	});
	jQuery("iframe.ytwd_main_iframe").each(function(index){
		var id = jQuery(this).attr("id");
	    var key = jQuery(this).attr("data-id");
		wdPlayers[index] = new YT.Player(id, {
			events: {
				'onReady': ytwdOnPlayerReady,
				'onError': ytwdOnPlayerError
			}
		});

	});	

}
	
function ytwdOnPlayerError(event){
	console.log('An error occurred: ' + event.data);
}

function ytwdOnPlayerReady(event){
	//console.log("ytwdOnPlayerReady");
	var player = event.target;
	var key = jQuery(player.a).attr("data-id");
	var number = jQuery(player.a).attr("data-number");
	wdTargetPlayers[key] = player;	
	jQuery(document).on("click", ".ytwd_gallery_item" + key + " .ytwd_item" ,function(event){
		event.preventDefault();		
		var resourceId = jQuery(this).attr("data-resource-id");
		var currentKey = Number(jQuery(this).attr("data-current"));
		jQuery(this).find("img").attr("src", ytwdGlobal.YTWD_URL + "/assets/loader.png");
		jQuery(this).find("img").addClass("ytwd_play_loader ytwd_loader");
		loadNewVideoID(resourceId, wdTargetPlayers[key], key);
		if(jQuery(".ytwd_popupbox_wrapper" + key).length == 0 && ytwdData["galleryViewType" + key] != "carousel"){
			var position = jQuery("#ytwd_main_iframe" + key).offset();
			jQuery('html,body').animate({
				scrollTop: position.top - 60
			});
		}
		
		ytwdGetVideoInfoData(key, resourceId, currentKey);
	});	
	
	if(number){
	   key = key.substr(number.length);
	}	

	player.setVolume(Number(ytwdData["initialVolume" + key]));
	if(ytwdData["videoQuality" + key]){
		player.setPlaybackQuality(ytwdData["videoQuality" + key]);	
	}	
}


function getPlayerVars(src){
	src = src.substr(src.indexOf("?") + 1);
	src = src.split("&");
	for(var i=0; i<src.length; i++){
		var playerVar = src[i].split("=");
		playerVars_[playerVar[0]] = playerVar[1];
	}
}
function loadNewVideoID(resourceId, player, key){
	player.destroy();
	wdTargetPlayers[key] = new YT.Player("ytwd_main_iframe" + key, {
		videoId: resourceId,
		width: '100%',			
		playerVars: playerVars_,
		events: {
			'onReady': ytwdOnPlayerReady,
			'onError': ytwdOnPlayerError
		}
	});	
}
function youTubeReadyFunction(key){
	createIframe = setInterval(function(){ 
		if( jQuery("div.ytwd_main_iframe").length > 0 ){
			onYouTubeIframeAPIReady();		
		}
		else{
			clearInterval(createIframe);
		}

	}, 1000);		
    jQuery('.ytwd_video_description').linkify();
    jQuery('.ytwd_channel_desc').linkify();
    if( ytwdData["embedType" + key] != 0 &&  ytwdData["enableGallery" + key] == 1){
		if(Number(jQuery("#ytwd_thumbnails" + key).width()) < Number(ytwdData["thumbsWidth" + key])){
			jQuery(".ytwd_gallery_item" + key).css("width", "100%");
		}
		
        // get hash from url
        var hash = window.location.hash;
		page = 1;
        if(hash.indexOf("youtubepage_") > -1){
            var page = hash.substr(hash.indexOf("youtubepage_"));
            page = Number(page.slice(12, page.indexOf("/") ));
        }
		ytwdPagination((page-1), 1, (page-1), key, true);
		
        // pagination
        jQuery(document).on("click", ".ytwd_pagination_btn" + key, function(){
            var page = Number(jQuery(this).attr("data-page"));
            var direction = jQuery(this).attr("data-direction");
            var currentPage = Number(jQuery(".ytwd_current_page" + key).html());
            ytwdPagination(page, direction, currentPage, key, false);

        });

        // load more
        jQuery(document).on("click", ".ytwd_load_more" + key, function(){
            var itemsCount = Number(jQuery(this).attr("data-limit"));

            jQuery('.ytwd_gallery_container' + key).css("opacity", "0.5");
            jQuery('.ytwd_loading_wrapper' + key).show();
            data = {};
            data["action"] = "ytwd_gallery_pagination";
            data["page"] = 0;
            data["items_count"] = itemsCount;

            jQuery.post(window.location, data, function (response){
                var gallery = jQuery(response).find('.ytwd_gallery_container' + key).html();
                jQuery('.ytwd_gallery_container' + key).html(gallery); 
				ytwdAdditionalInfoHeight(key);
                jQuery(".ytwd_load_more" + key).attr("data-limit", itemsCount + ytwdData["itemsCountPerPage" + key]);
                jQuery('.ytwd_gallery_container' + key).css("opacity", "1");
                jQuery('.ytwd_loading_wrapper' + key).hide();
				ytwdLoadingEffects(key, false);
				onYouTubeIframeAPIReady();
				if(Number(jQuery(".ytwd_gallery_item" + key).length)  == ytwdData["totalPages" + key]){
					jQuery(".ytwd_load_more_wrapper" + key).hide();
				}

            });

        });


		// loading effects
		ytwdLoadingEffects(key, true);

    }
    // comments
    jQuery(document).on("click", ".ytwd_load_toggle" + key, function(){
        if(jQuery(".ytwd_more_info" + key).is(":visible") == true){
            jQuery(this).html(ytwdGlobal.txt_show_more);
			jQuery(this).closest(".ytwd_show_more_divaider").removeAttr("style");
			jQuery(".ytwd_short_more_info" + key).show();
        }
        else{
            jQuery(this).html(ytwdGlobal.txt_show_less);
			jQuery(this).closest(".ytwd_show_more_divaider").css("border-bottom", "none");
			jQuery(".ytwd_short_more_info" + key).hide();
        }
        jQuery(".ytwd_more_info" + key).toggle();
       
    });

	
	// set info height

	ytwdAdditionalInfoHeight(key);
	
	
	jQuery(window).resize(function(){
		ytwdAdditionalInfoHeight(key);
	});	
}



function ytwdLoadingEffects(key, scrollToTop){
	
	if(ytwdData["loadingEffects" + key] != "none"){
		if(scrollToTop == true){
			jQuery(window).scrollTop(0);
		}
		jQuery(".ytwd_gallery_item" + key).each(function() {
			var objectOffsetTop = jQuery(this).offset().top;
			var windowInnerHeight = jQuery(window).innerHeight() + jQuery(window).scrollTop();
			if (objectOffsetTop > windowInnerHeight) {
				jQuery(this).css("opacity", "0");
				jQuery(this).addClass("ytwd_gallery_item_opacity" + key);
			}
		});

		jQuery(window).scroll(function() {
			jQuery(".ytwd_gallery_item_opacity" + key).each(function() {
				  var objectOffsetTop = jQuery(this).offset().top;
				  var windowHeight = jQuery(window).scrollTop() + jQuery(window).innerHeight();
				  if (objectOffsetTop < windowHeight) {
					if(ytwdData["loadingEffects" + key] == "fade_in"){
						jQuery(this).fadeTo(500,1);
					}
				  }
			});
		});
	}		
}

function ytwdGetVideoInfoData(key, resourceId, currentKey){
	jQuery("#ytwd_main_iframe" + key).removeAttr("data-clicked");	
	if(ytwdData["videoAdditionalInfo" + key]){	
	    data = {};
	    data["action"] = "ytwd_gallery_search";
	    data["resource_id"] = resourceId;
		jQuery('.ytwd_main_video_info' + key).css("opacity", "0.3");
		jQuery('.main_video_loder' + key).show();
	    jQuery.post(window.location, data, function (response){
			jQuery.getScript("https://apis.google.com/js/platform.js");
	        var moreInfo = jQuery(response).find('.ytwd_main_video_info' + key).html();
	        jQuery('.ytwd_main_video_info' + key).html(moreInfo);
			jQuery('.main_video_loder' + key).hide();
			jQuery('.ytwd_main_video_info' + key).css("opacity", "1");

	        if(jQuery(".ytwd_popupbox_wrapper" + key).length > 0 ){
				if(key != "preview"){
					if(jQuery(".ytwd_main_video_info" + key).children().length > 0){
						jQuery(".ytwd_main_video_info" + key).addClass("ytwd_for_popup");
						jQuery(".ytwd_popupbox" + key).attr("style","top:47px !important; height:auto !important");
					}
					var nextKey = (currentKey + 1) < ytwdItems[key].length ? currentKey + 1 : currentKey;
					var prevKey = (currentKey - 1) >= 0 ? currentKey -1 : currentKey;
					jQuery(".ytwd_next_btn" + key).attr("data-step", nextKey);
					jQuery(".ytwd_prev_btn" + key).attr("data-step", prevKey);
					jQuery(".ytwd_popupbox_wrapper" + key).show();
					jQuery("html,body").addClass("ytwd_overflow_hidden");
				}
	        }	
	        jQuery(".ytwd_gallery_item" + key + " .ytwd_play").removeClass("ytwd_play_loader ytwd_loader");
	        jQuery(".ytwd_gallery_item" + key + " .ytwd_item[data-current=" + currentKey + "]").find("img").attr("src", ytwdGlobal.YTWD_URL + "/assets/play.png");

	    });
	}
	else{
        if(jQuery(".ytwd_popupbox_wrapper" + key).length > 0 ){
			if(key != "preview"){
				if(jQuery(".ytwd_main_video_info" + key).children().length > 0){
					jQuery(".ytwd_main_video_info" + key).addClass("ytwd_for_popup");
					jQuery(".ytwd_popupbox" + key).attr("style","top:47px !important; height:auto !important");
				}
				var nextKey = (currentKey + 1) < ytwdItems[key].length ? currentKey + 1 : currentKey;
				var prevKey = (currentKey - 1) >= 0 ? currentKey -1 : currentKey;
				jQuery(".ytwd_next_btn" + key).attr("data-step", nextKey);
				jQuery(".ytwd_prev_btn" + key).attr("data-step", prevKey);
				jQuery(".ytwd_popupbox_wrapper" + key).show();
				jQuery("html,body").addClass("ytwd_overflow_hidden");
			}
        }		
        jQuery(".ytwd_gallery_item" + key + " .ytwd_play").removeClass("ytwd_play_loader ytwd_loader");
        jQuery(".ytwd_gallery_item" + key + " .ytwd_item[data-current=" + currentKey + "]").find("img").attr("src", ytwdGlobal.YTWD_URL + "/assets/play.png");		
	}	
}

function ytwdPagination(page, direction, currentPage, key, load){

    var pageNum = Number(jQuery(".ytwd_num_pages" + key).html());
    if((direction == 1 && currentPage < pageNum) || (direction == -1 && currentPage <= pageNum) || load == true ){
        jQuery('.ytwd_gallery_container' + key).css("opacity", "0.5");
        jQuery('.ytwd_loading_wrapper' + key).show();
        data = {};
        data["action"] = "ytwd_gallery_pagination";
        data["page"] = page;

        jQuery.post(window.location, data, function (response){
            var gallery = jQuery(response).find('.ytwd_gallery_container' + key).html();
            jQuery('.ytwd_gallery_container' + key).html(gallery);
			
			ytwdAdditionalInfoHeight(key);  
            if(page){
                jQuery(".ytwd_pagination_btn" + key + "[data-direction='+1']").attr("data-page", page + 1);
                jQuery(".ytwd_pagination_btn" + key + "[data-direction='-1']").attr("data-page", page - 1);

                jQuery(".ytwd_current_page" + key).html(eval(currentPage + direction));
            }
			pageNum = Number(jQuery(".ytwd_num_pages" + key).html());
			if(page + 1 == pageNum ){
				jQuery(".ytwd_pagination_btn_next" + key).hide();
				jQuery(".ytwd_pagination_btn_prev" + key).show();
			}			
			else if(page == 0){
				jQuery(".ytwd_pagination_btn_next" + key).show();
				jQuery(".ytwd_pagination_btn_prev" + key).hide();
			}
			else{
				jQuery(".ytwd_pagination_btn_next" + key).show();
				jQuery(".ytwd_pagination_btn_prev" + key).show();				
			}			
            jQuery('.ytwd_gallery_container' + key).css("opacity", "1");
            jQuery('.ytwd_loading_wrapper' + key).hide();
			if(load == false){
				if(window.location.hash.indexOf("youtubepage_") > -1){
					var ytwdHash = window.location.hash.substr(window.location.hash.indexOf("youtubepage_"));
					ytwdPage = ytwdHash.slice(12, ytwdHash.indexOf("/") );
					window.location.hash = window.location.hash.replace("youtubepage_" + ytwdPage , "youtubepage_" + (Number(page) + 1) );
				}
				else{
					window.location.hash = window.location.hash + "youtubepage_" + (Number(page) + 1) + "/";
				}				
			}
			onYouTubeIframeAPIReady();
			ytwdLoadingEffects(key, false);
			
        });
    }
}


function ytwdAdditionalInfoHeight(key){

    var maxItemTitileHeight = 0;
    jQuery(".ytwd_item_title" + key).each(function(){
        if(jQuery(this).height() > maxItemTitileHeight){
            maxItemTitileHeight = jQuery(this).height();
        }
    });
    jQuery(".ytwd_item_title" + key).height(maxItemTitileHeight);
    var maxItemPublishedAtHeight = 0;
    jQuery(".ytwd_item_pub_at" + key).each(function(){
        if(jQuery(this).height() > maxItemPublishedAtHeight){
            maxItemPublishedAtHeight = jQuery(this).height();
        }
    });
    jQuery(".ytwd_item_pub_at" + key).height(maxItemPublishedAtHeight);

    var maxItemDescHeight = 0;
    jQuery(".ytwd_item_desc" + key).each(function(){
        if(jQuery(this).height() > maxItemDescHeight){
            maxItemDescHeight = jQuery(this).height();
        }
    });
    jQuery(".ytwd_item_desc" + key).height(maxItemDescHeight);

}




////////////////////////////////////////////////////////////////////////////////////////
// Getters & Setters                                                                  //
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
// Private Methods                                                                    //
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// Listeners                                                                          //
////////////////////////////////////////////////////////////////////////////////////////
