#pragma once

#include "PluginWindow.h"
#include "PluginEvents/MouseEvents.h"
#include "PluginEvents/AttachedEvent.h"
#include "PluginEvents/DrawingEvents.h"

#include "PluginCore.h"

#include <libvlc_wrapper/vlc_player.h>

#include "vlc_player_options.h"

FB_FORWARD_PTR( Chimera )
class Chimera 
    : public FB::PluginCore,
      protected vlc_player,
      protected vlc_player_options
{
public:
    Chimera();
    virtual ~Chimera();

protected:
    void onPluginReady();
    void shutdown();
    // If you want your plugin to always be windowless, set this to true
    // If you want your plugin to be optionally windowless based on the
    // value of the "windowless" param tag, remove this method or return
    // FB::PluginCore::isWindowless()
    virtual bool isWindowless() { return false; }

    BEGIN_PLUGIN_EVENT_MAP()
    END_PLUGIN_EVENT_MAP()

private:
    /** BEGIN EVENTDEF -- DON'T CHANGE THIS LINE **/
    /** END EVENTDEF -- DON'T CHANGE THIS LINE **/

public:
    libvlc_instance_t* getLibVlc()
        { return m_libvlc; };
    vlc_player& get_player()
        { return *static_cast<vlc_player*>( this ); };
    vlc_player_options& get_options()
        { return *static_cast<vlc_player_options*>( this ); }
    const vlc_player_options& get_options() const
        { return *static_cast<const vlc_player_options*>( this ); }

    int add_playlist_item( const std::string& mrl );
    int add_playlist_item( const std::string& mrl, const std::vector<std::string>& options );

public:
    virtual bool is_fullscreen() { return false; };
    virtual void set_fullscreen( bool fs ) {};
    virtual void toggle_fullscreen()
        { set_fullscreen( !is_fullscreen() ); };

private:
    std::string detectHttpProxy( const std::string& mrl ) const;

protected:
    const FB::variant& getParamVariant( const std::string& key ) const;

    virtual void load_startup_options();
    void vlc_open();
    virtual void apply_player_options();
    void vlc_close();

    virtual bool isOptionTrusted( const std::string& option )
        { return false; };

    virtual void on_option_change( vlc_player_option_e );

private:
    void load_libvlc_options();

    static void OnLibVlcEvent_proxy( const libvlc_event_t* e, void* param );
    void OnLibVlcEvent( const libvlc_event_t* e );
    void VlcEvents( bool Attach );

    void onMediaPlayerPlaying();

private:
    libvlc_instance_t* m_libvlc;
    bool m_forceMute;
};
