#pragma once

#include <jzon/Jzon.h>

#include "../../object/resource.h"

namespace ogele {
    class Texture2DLoader : public ResourceLoader {

    public:

        Resource *Load(const Jzon::Node *reader) const override;

    };
}