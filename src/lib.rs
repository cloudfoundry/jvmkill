#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]

use std::sync::Mutex;

mod env;
mod jvmti;
mod agentcontroller;

use env::JVMTI;

#[macro_use]
extern crate lazy_static;
extern crate libc;

lazy_static! {
    static ref STATIC_CONTEXT: Mutex<AgentContext> = Mutex::new(AgentContext::new());
    static ref RAW_MONITOR_ID: Mutex<env::RawMonitorID> = Mutex::new(env::RawMonitorID::new());
}

pub struct AgentContext {
    ac: Option<agentcontroller::agentController>
}

impl AgentContext {
    pub fn new() -> AgentContext {
        AgentContext {
            ac: None,
        }
    }

    pub fn set(&mut self, a: agentcontroller::agentController) {
        match self.ac {
            Some(_) => panic!("Agent controller has already been set"),
            None => self.ac = Some(a),
        }
    }
}

#[no_mangle]
#[allow(unused_variables)]
pub extern fn Agent_OnLoad(vm: *mut jvmti::JavaVM, options: *mut ::std::os::raw::c_char,
                           reserved: *mut ::std::os::raw::c_void) -> jvmti::jint {
    let jvmti_env = env::JVMTIEnv::new(vm);
    let ac = jvmti_env.and_then(|ti| agentcontroller::agentController::new(ti));

    match ac {
        Ok(a) =>
            STATIC_CONTEXT.lock().unwrap().set(a),
        Err(e) => {
            return 1;
        }
    }

    let rc: Result<(), jvmti::jint> = jvmti_env.and_then(|mut ti| {
        ti.OnResourceExhausted(resourceExhausted);

        ti.CreateRawMonitor(String::from("jvmkill"), &RAW_MONITOR_ID)});

    match rc {
        Ok(_) => 0,
        Err(e) => e
    }
}

fn resourceExhausted(jvmti_env: env::JVMTIEnv, flags: ::jvmti::jint) {
    println!("Resource exhausted callback driven!")
}


/*
int setCallbacks(jvmtiEnv *jvmti) {
   jvmtiError err;

   err = jvmti->CreateRawMonitor("jvmkillMonitor", &monitorID);
   if (err != JVMTI_ERROR_NONE) {
      std::cerr << "ERROR: CreateRawMonitor failed: " << err << std::endl;
      return JNI_ERR;
   }

   jvmtiEventCallbacks callbacks;
   memset(&callbacks, 0, sizeof(callbacks));

   callbacks.ResourceExhausted = &resourceExhausted;

   err = jvmti->SetEventCallbacks(&callbacks, sizeof(callbacks));
   if (err != JVMTI_ERROR_NONE) {
      std::cerr << "ERROR: SetEventCallbacks failed: " << err << std::endl;
      return JNI_ERR;
   }

   err = jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_RESOURCE_EXHAUSTED, NULL);
   if (err != JVMTI_ERROR_NONE) {
      std::cerr << "ERROR: SetEventNotificationMode failed: %d" << err << std::endl;
      return JNI_ERR;
   }

   return JNI_OK;
}

*/
