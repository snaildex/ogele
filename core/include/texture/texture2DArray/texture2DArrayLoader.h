#pragma once

#include <jzon/Jzon.h>

#include "../../object/resource.h"

namespace ogele {
    class Texture2DArrayLoader : public ResourceLoader {

    public:

        Resource *Load(const Jzon::Node *reader) const override;

    };
}