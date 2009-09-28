/*

  Much of this code was originally based of Lennart Pottering's pavumeter.
 
  pavumeter is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published
  by the Free Software Foundation; either version 2 of the License,
  or (at your option) any later version.
 
  pavumeter is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with pavumeter; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
  USA.
 */ 
#include <pulse/pulseaudio.h>
#include <pulse/glib-mainloop.h>

#include <iostream>

#include "pasource.h"

// forwards

static void context_state_callback(pa_context *c, void * pasrcptr);

PASource::PASource() :
  mainloop_(NULL),
  stream_(NULL),
  context_(NULL),
  mode_(PLAYBACK), 
  device_name_(NULL),
  device_description_(NULL)
{
  mainloop_ = pa_glib_mainloop_new(g_main_context_default());
  g_assert(mainloop_);
            
  context_ = pa_context_new(pa_glib_mainloop_get_api(mainloop_), "PulseAudio Soma Streams Monitor");
  g_assert(context_);
            
  pa_context_set_state_callback(context_, context_state_callback, this);
  pa_context_connect(context_, NULL, PA_CONTEXT_NOAUTOSPAWN, NULL);

}

PASource::~PASource()
{
  std::cout << "Beginning destruction" << std::endl;
  pa_glib_mainloop_free(mainloop_);
            
  if (stream_)
    pa_stream_unref(stream_);

//   if (context_)
//     pa_context_unref(context_);
            
}


void PASource::pushData(const float * f, unsigned l)
{
  datasig.emit(f, l); 
}

// static void stream_update_timing_info_callback(pa_stream *s, int success, void *) {
//     pa_usec_t t;
//     int negative = 0;
    
//     if (!success || pa_stream_get_latency(s, &t, &negative) < 0) {
//         show_error("Failed to get latency information");
//         return;
//     }

//     if (!mainWindow)
//         return;

//     mainWindow->updateLatency(negative ? 0 : t);
// }

// static gboolean latency_func(gpointer) {
//     pa_operation *o;
    
//     if (!stream)
//         return false;

//     if (!(o = pa_stream_update_timing_info(stream, stream_update_timing_info_callback, NULL)))
//         g_message("pa_stream_update_timing_info() failed: %s", pa_strerror(pa_context_errno(context)));
//     else
//         pa_operation_unref(o);
    
//     return true;
// }

static void stream_read_callback(pa_stream *s, size_t l, void * ptr) {
  PASource * pasource = (PASource *)ptr; 
  
  const void *p;
  if (pa_stream_peek(s, &p, &l) < 0) {
    std::cerr << "pa_stream_peek() failed: %s" <<  pa_strerror(pa_context_errno(pasource->context_))
	      << std::endl;
    return;
  }
    
  pasource->pushData((const float*) p, l/sizeof(float));

  pa_stream_drop(s);
}

static void stream_state_callback(pa_stream *s, void * ptr) {
  PASource * pasource = (PASource *)ptr; 

    switch (pa_stream_get_state(s)) {
        case PA_STREAM_UNCONNECTED:
        case PA_STREAM_CREATING:
            break;

        case PA_STREAM_READY:

	  //mainWindow = new MainWindow(*pa_stream_get_channel_map(s), device_name, device_description);

	  //g_timeout_add(100, latency_func, NULL);
	  //pa_operation_unref(pa_stream_update_timing_info(stream, stream_update_timing_info_callback, NULL));
            break;
            
        case PA_STREAM_FAILED:
	  std::cerr << "Connection failed" << std::endl;
            break;
            
        case PA_STREAM_TERMINATED:
	  //Gtk::Main::quit();
	  break; 
    }
}

static void create_stream(const char *name, const char *description, const pa_sample_spec &ss, const pa_channel_map &cmap,
			  PASource* pasource) {
    char t[256];

    
    g_free(pasource->device_name_);
    pasource->device_name_ = g_strdup(name);
    g_free(pasource->device_description_);
    pasource->device_description_ = g_strdup(description);
    
    pasource->nss_.format = PA_SAMPLE_FLOAT32;
    pasource->nss_.rate = ss.rate;
    pasource->nss_.channels = ss.channels;
    
//     g_message("Using sample format: %s", pa_sample_spec_snprint(t, sizeof(t), &nss));
//     g_message("Using channel map: %s", pa_channel_map_snprint(t, sizeof(t), &cmap));

    pasource->stream_ = pa_stream_new(pasource->context_, "PulseAudio Volume Meter", 
				      &pasource->nss_, &cmap);
    pa_stream_set_state_callback(pasource->stream_, stream_state_callback, pasource);
    pa_stream_set_read_callback(pasource->stream_, stream_read_callback, pasource);
    pa_stream_connect_record(pasource->stream_, name, NULL, (enum pa_stream_flags) 0);
}

static void context_get_source_info_callback(pa_context *, const pa_source_info *si, int is_last, void * p) {
  PASource * pasource = (PASource *)p; 
  if (is_last < 0) {
    std::cerr << "Failed to get source information" << std::endl;
    return;
  }
  
  if (!si)
    return;
  
  create_stream(si->name, si->description, si->sample_spec, 
		si->channel_map, pasource);
}

static void context_get_sink_info_callback(pa_context *, const pa_sink_info *si, int is_last, void * p) {
  PASource * pasource = (PASource *)p; 
  if (is_last < 0) {
    std::cerr << "Failed to get sink information" << std::endl; 
    return;
    }
  
  if (!si)
    return;
  
  create_stream(si->monitor_source_name, si->description, 
		si->sample_spec, si->channel_map, pasource);
}

static void context_get_server_info_callback(pa_context *c, const pa_server_info*si, void * p)  {
  PASource * pasource = (PASource *)p; 
  
  if (!si) {
    std::cerr << "Failed to get server information" << std::endl;
    return;
  }

  if (pasource->mode_ == PASource::PLAYBACK) {
    std::cout << "Server info callback PLAYBACK" << std::endl; 
    if (!si->default_sink_name) {
      std::cerr << "No default sink set." << std::endl; 
      return;
    }
    
    pa_operation_unref(pa_context_get_sink_info_by_name(c, si->default_sink_name, context_get_sink_info_callback, p));
    
  } else if (pasource->mode_ == PASource::RECORD) {
    std::cout << "Server info callback RECORD" << std::endl; 
    
    if (!si->default_source_name) {
      std::cerr << "No default source set." << std::endl; 
      return;
    }
    
    pa_operation_unref(pa_context_get_source_info_by_name(c, si->default_source_name, context_get_source_info_callback, p));
  }
}

static void context_state_callback(pa_context *c, void * pasrcptr) {
  PASource * pasource = (PASource *)pasrcptr; 
  std::cout << "context state callback" << std::endl;
    switch (pa_context_get_state(c)) {
        case PA_CONTEXT_UNCONNECTED:
        case PA_CONTEXT_CONNECTING:
        case PA_CONTEXT_AUTHORIZING:
        case PA_CONTEXT_SETTING_NAME:
            break;

        case PA_CONTEXT_READY: 
	  {
	    g_assert(!(pasource->stream_));
	    
	    //             if (device_name && mode == RECORD) {
	    // 	      std::cout << "Doing the record dance" << std::endl; 
	    // 	      pa_operation_unref(pa_context_get_source_info_by_name(c, device_name, context_get_source_info_callback, NULL));
// 	    } 
//             else if (device_name && mode == PLAYBACK) {
// 	      std::cout << "Doing the playback dance" << std::endl; 
// 	      pa_operation_unref(pa_context_get_sink_info_by_name(c, device_name, context_get_sink_info_callback, NULL)); 
// 	    } 
//             else {
	    std::cout << "get server info in general " << std::endl; 
	    
	    pa_operation_unref(pa_context_get_server_info(c, context_get_server_info_callback, pasrcptr));
	    //            }
	  }
            break;
            
        case PA_CONTEXT_FAILED:
	  std::cerr << "Connection failed" << std::endl; 
	  break;
	  
        case PA_CONTEXT_TERMINATED: 
	  {
	  std::cerr << "PA Context terminated" << std::endl; 
	  
	  }
          
    }
    std::cout << "Context state callback end" << std::endl; 
}
