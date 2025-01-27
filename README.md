# Test header Module for Nginx

## Introduction

Develop an nginx module that:
- is configured by a directive in nginx.conf specifying the name of the HTTP header. For example: test_header_name Test-Header;
- reads the value of the test_header_name request header, initializes the nginx variable. 

## Installation

   1. Configure Nginx adding this module with:
          
          Static Module : ./configure (...) --add-module=/path/to/nginx-test-header-module
          Dynamic Module: ./configure (...) --add-dynamic-module=/path/to/nginx-test-header-module
       
   2. Build Nginx as usual with `make`.
   
   3. Configure the module:
      
      open nginx.conf:
          
        location / {

            test_header Test-Header;

        }


      Now request data from server:
          
          curl -i localhost 2>&1 | grep Test-Header
          
       **Test-Header: 42** example response body.
      
## TODO

    Make variable visible in another module to satisfy these cases:
    - This value must be written to the access log described before response to client. 
    - The random number transmitted to the client must also be written to the access log.
 
	
    Could be achieved by using ngx_http_add_variable to create shared variable
	add define setter and getters:
	ngx_http_set_variable_pt
	ngx_http_get_variable_pt
	
	Log module should use ngx_http_get_variable to retreive the variable and use getter above
	
	In log module I defined new hadler on phase NGX_HTTP_POST_READ_PHASE by ngx_http_request_handler
	
	