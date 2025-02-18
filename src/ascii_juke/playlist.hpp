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

#include <cstddef>
#include <string>

namespace AsciiJuke {
struct PlaylistEntry {
    size_t id;
    std::string name;
    std::string file_path;
    PlaylistEntry *next;
};

typedef enum {
    PLAYLIST_OP_OK = 0,
    PLAYLIST_OP_ENTRY_ALREADY_EXISTS,
    PLAYLIST_OP_INVALID_ENTRY,
    PLAYLIST_OP_INVALID_ID,
    PLAYLIST_OP_LIST_EMPTY
} PlaylistOperationResult;

class Playlist {
  private:
    PlaylistEntry *head;

    public:
    Playlist();
    ~Playlist();

    PlaylistOperationResult add_entry(size_t id, std::string name, std::string file_path);
    PlaylistOperationResult remove_entry(size_t id);
};
} // namespace AsciiJuke