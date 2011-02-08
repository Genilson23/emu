#include <iostream>
#include "viewport.h"
#include "protocol.h"
#include "gameobject.h"
#include "character.h"

viewportManager_t::viewportManager_t(protocol_t &protocol):
  m_protocol(protocol) {}

void viewportManager_t::generate(gameObject_t &object) { _PROFILE;
	for(std::list<gameObject_t*>::iterator i = m_objectList.begin(); i != m_objectList.end(); ++i) {
		gameObject_t *checkedObject = (*i);

		if(&object == checkedObject || checkedObject->getMapId() != object.getMapId()) {
			continue;
		}

		unsigned int distance = object.calculateDistance(*checkedObject);

		if(distance <= checkedObject->getViewRange()) {
			if(checkedObject->getViewport().insert(&object).second) {
				//std::cout << "[viewportManager_t::generate()] " << *checkedObject << " saw " << object << ".\n";

				// -----------------------------------------------------------------------------------
				if(checkedObject->getType() == gameObject_t::_OBJECT_CHARACTER) {
					character_t &character = reinterpret_cast<character_t&>(*checkedObject);

					viewport_t viewed;
					viewed.insert(&object);

					if(object.getType() == gameObject_t::_OBJECT_CHARACTER) {
						m_protocol.sendViewportCharacterCreateRequest(character.getOwner(), viewed);
					} else {
						m_protocol.sendViewportMonsterCreateRequest(character.getOwner(), viewed);
					}
				}
				// -----------------------------------------------------------------------------------
			}
		}

		if(distance <= object.getViewRange()) {
			if(object.getViewport().insert(checkedObject).second) {
				//std::cout << "[viewportManager_t::generate()] " << object << " saw " << *checkedObject << ".\n";

				// jest to tymczasowy HACK, poniewaz klient webzen renderuje smieci
				// kiedy po pierwszym wejsciu postaci wysylamy mu liste widzianych obiektow.
				// Trzeba pojedynczo. Byc moze na nowszych klientach nie ma tego problemu.
				// -------------------------------------------------------------------------------
				if(object.getType() == gameObject_t::_OBJECT_CHARACTER) {
					character_t &character = reinterpret_cast<character_t&>(object);

					viewport_t viewed;
					viewed.insert(checkedObject);

					if(checkedObject->getType() == gameObject_t::_OBJECT_CHARACTER) {
						m_protocol.sendViewportCharacterCreateRequest(character.getOwner(), viewed);
					} else {
						m_protocol.sendViewportMonsterCreateRequest(character.getOwner(), viewed);
					}
				}
				// -------------------------------------------------------------------------------
			}
		}
	}
}

void viewportManager_t::update(gameObject_t &object) { _PROFILE;
	for(viewport_t::iterator i = object.getViewport().begin(); i != object.getViewport().end();) {
		gameObject_t *checkedObject = (*i);
		unsigned int distance = object.calculateDistance(*checkedObject);

		if(distance > checkedObject->getViewRange() || checkedObject->getMapId() != object.getMapId()) {
			viewport_t::iterator it = checkedObject->getViewport().find(&object);

			if(it != checkedObject->getViewport().end()) {
				//std::cout << "[viewportManager_t::update()] " << *checkedObject << " forgot " << object << ".\n";

				// --------------------------------------------------------------------------
				if(checkedObject->getType() == gameObject_t::_OBJECT_CHARACTER) {
					character_t &character = reinterpret_cast<character_t&>(*checkedObject);

					viewport_t forgot;
					forgot.insert(&object);
					m_protocol.sendViewportDestroyRequest(character.getOwner(), forgot);
				}
				// --------------------------------------------------------------------------

				checkedObject->getViewport().erase(it);
			}
		}

		if(distance > object.getViewRange() || object.getMapId() != checkedObject->getMapId()) {
			//std::cout << "[viewportManager_t::update()] " << object << " forgot " << *checkedObject << ".\n";

			i = object.getViewport().erase(i);

			viewport_t forgot;
			forgot.insert(checkedObject);

			// ----------------------------------------------------------------------
			if(object.getType() == gameObject_t::_OBJECT_CHARACTER) {
				character_t &character = reinterpret_cast<character_t&>(object);
				m_protocol.sendViewportDestroyRequest(character.getOwner(), forgot);
			}
			// ----------------------------------------------------------------------
		} else {
			++i;
		}
	}

	this->generate(object);
}

void viewportManager_t::clear(gameObject_t &object) { _PROFILE;
	for(viewport_t::iterator i = object.getViewport().begin(); i != object.getViewport().end(); ++i) {
		gameObject_t *checkedObject = (*i);
		viewport_t::iterator it = checkedObject->getViewport().find(&object);

		if(it != checkedObject->getViewport().end()) {
			//std::cout << "[viewportManager_t::clear()] " << *checkedObject << " forgot " << object << ".\n";

			// --------------------------------------------------------------------------
			if(checkedObject->getType() == gameObject_t::_OBJECT_CHARACTER) {
				character_t &character = reinterpret_cast<character_t&>(*checkedObject);

				viewport_t forgot;
				forgot.insert(&object);
				m_protocol.sendViewportDestroyRequest(character.getOwner(), forgot);
			}
			// --------------------------------------------------------------------------

			checkedObject->getViewport().erase(it);
		}

		//std::cout << "[viewportManager_t::clear()] " << object << " forgot " << *checkedObject << ".\n";
	}

	// --------------------------------------------------------------------------------------
	if(object.getType() == gameObject_t::_OBJECT_CHARACTER) {
		character_t &character = reinterpret_cast<character_t&>(object);

		if(character.isActive()) {
			m_protocol.sendViewportDestroyRequest(character.getOwner(), character.getViewport());
		}
	}
	// --------------------------------------------------------------------------------------

	object.getViewport().clear();
}