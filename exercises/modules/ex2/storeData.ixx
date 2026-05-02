// This partition of the Store module defines the Order struct and is exported,
// making it visible to users of the Store module.
// It does not need changes and should remain as is for the exercise to work correctly.
export module Store:Data;

export namespace Store {
    struct Order {
        int id;
        double totalPrice;
    };
}