#pragma once

// For MSI.
struct Message {
  enum Type {
    kInterruptXHCI,
    kInterruptLAPICTimer,
  } type;
};
