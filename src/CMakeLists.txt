set(qcoap_srcs coappdu.cpp coapnode.cpp coapbridge.cpp dqobject.cpp)
set(qcoap_headers coap_global.h coappdu.h coapnode.h coapbridge.h)

add_library(qcoap SHARED ${qcoap_srcs} ${qcoap_headers})

if (Qt5Core_FOUND)
	target_link_libraries(qcoap Qt5::Core Qt5::Network)
else ()
	target_link_libraries(qcoap ${QT_LIBRARIES})
endif ()


if (NOT android)
	set_target_properties(qcoap PROPERTIES
				       VERSION ${QCOAP_MAJOR}.${QCOAP_MINOR}.${QCOAP_VERSION}
				       SOVERSION ${QCOAP_MAJOR})
endif ()
set_target_properties(qcoap PROPERTIES
				DEFINE_SYMBOL	COAP_MAKE_LIBRARY
				PUBLIC_HEADER	"${qcoap_headers}")

install(TARGETS qcoap EXPORT qcoap-export
	LIBRARY DESTINATION ${LIB_INSTALL_DIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
	PUBLIC_HEADER DESTINATION ${INCLUDE_INSTALL_DIR}/qcoap)
