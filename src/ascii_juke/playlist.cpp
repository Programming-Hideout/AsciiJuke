// AsciiJuke
// Copyright (C) 2024  Programming Hideout
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "ascii_juke/playlist.hpp"

namespace AsciiJuke {

Playlist::Playlist() {
    this->head = nullptr;
}

Playlist::~Playlist() {
    PlaylistEntry *current = this->head;
    while (current) {
        PlaylistEntry *temp = current;
        current = current->next;
        delete temp;
    }
}

PlaylistOperationResult Playlist::add_entry(size_t id, std::string name, std::string file_path) {
    PlaylistEntry *current = this->head;
    while (current) {
        if (current->id == id) {
            return PLAYLIST_OP_ENTRY_ALREADY_EXISTS;
        }
        current = current->next;
    }

    PlaylistEntry *entry = new PlaylistEntry;
    entry->id = id;
    entry->name = name;
    entry->file_path = file_path;

    entry->next = this->head;
    this->head = entry;
    return PLAYLIST_OP_OK;
}

PlaylistOperationResult Playlist::remove_entry(size_t id) {
    PlaylistEntry *current = this->head;
    while (current) {
        if (current->id == id) {
            return PLAYLIST_OP_OK;
        }
        current = current->next;
    }

    return PLAYLIST_OP_INVALID_ID;
}
} // namespace AsciiJuke