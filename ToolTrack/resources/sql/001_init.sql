-- =============================================
-- SCHEMA_MIGRATIONS TABLE
-- =============================================
CREATE TABLE IF NOT EXISTS schema_migrations (
    id INTEGER PRIMARY KEY,
    filename TEXT UNIQUE NOT NULL,
    applied_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

-- =============================================
-- USERS TABLE
-- =============================================
CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY,
    first_name VARCHAR(50) NOT NULL,
    last_name VARCHAR(50) NOT NULL,
    barcode VARCHAR(50) NOT NULL UNIQUE
);

-- =============================================
-- ITEM_TYPES TABLE
-- =============================================
CREATE TABLE IF NOT EXISTS item_types (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name VARCHAR(50) NOT NULL UNIQUE
);

-- =============================================
-- ITEM_STATES TABLE
-- =============================================
CREATE TABLE IF NOT EXISTS item_states (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name VARCHAR(50) NOT NULL UNIQUE
);

-- =============================================
-- OWNER_TYPES TABLE
-- =============================================
CREATE TABLE IF NOT EXISTS owner_types (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name VARCHAR(50) NOT NULL UNIQUE
);

-- =============================================
-- PERSONS TABLE
-- =============================================
CREATE TABLE IF NOT EXISTS persons (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    first_name VARCHAR(50) NOT NULL,
    last_name VARCHAR(50) NOT NULL
);

-- =============================================
-- INSTITUTIONS TABLE
-- =============================================
CREATE TABLE IF NOT EXISTS institutions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name VARCHAR(120) NOT NULL UNIQUE
);

-- =============================================
-- LOCATIONS TABLE
-- =============================================
CREATE TABLE IF NOT EXISTS locations (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name VARCHAR(120) NOT NULL UNIQUE
);


-- =============================================
-- ITEMS TABLE
-- =============================================
CREATE TABLE IF NOT EXISTS items (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name VARCHAR(50) NOT NULL,
    barcode VARCHAR(50) NOT NULL UNIQUE,

    type_id INTEGER NOT NULL,
    state_id INTEGER NOT NULL,

    stock INTEGER NOT NULL DEFAULT 0,
    brand VARCHAR(50),
    model VARCHAR(50),

    owner_type_id INTEGER,
    owner_id INTEGER,

    location_id INTEGER,
    notes TEXT,

    FOREIGN KEY (type_id) REFERENCES item_types(id),
    FOREIGN KEY (state_id) REFERENCES item_states(id),
    FOREIGN KEY (owner_type_id) REFERENCES owner_types(id),
    FOREIGN KEY (location_id) REFERENCES locations(id)
);

-- =============================================
-- ITEMS_CHECKOUT TABLE
-- =============================================
CREATE TABLE IF NOT EXISTS items_checkout (
    id INTEGER PRIMARY KEY,
    original_id INTEGER,
    user_id INTEGER,
    item_id INTEGER,
    checkout_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    returned_at TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
    FOREIGN KEY (item_id) REFERENCES items(id) ON DELETE CASCADE
);

-- =============================================
-- ITEMS_CHECKOUT_ARCHIVE TABLE
-- =============================================
CREATE TABLE IF NOT EXISTS items_checkout_archive (
    id INTEGER PRIMARY KEY,
    user_id INTEGER NOT NULL,
    item_id INTEGER NOT NULL,
    checkout_at TIMESTAMP NOT NULL,
    returned_at TIMESTAMP,
    archived_at TIMESTAMP NOT NULL,
    archive_reason TEXT
);



--------------------------------------------------------------------
-- USERS
--------------------------------------------------------------------
-- Búsqueda rápida por barcode (identificación mediante escáner)
CREATE INDEX IF NOT EXISTS idx_users_barcode ON users(barcode);

-- Nombre + apellido (útil para ordenación y búsquedas parciales)
CREATE INDEX IF NOT EXISTS idx_users_name ON users(last_name, first_name);



--------------------------------------------------------------------
-- PERSONS
--------------------------------------------------------------------
-- Orden y búsqueda por nombre
CREATE INDEX IF NOT EXISTS idx_persons_name ON persons(last_name, first_name);



--------------------------------------------------------------------
-- LOCATIONS
--------------------------------------------------------------------
-- Búsqueda por nombre
CREATE INDEX IF NOT EXISTS idx_locations_name ON locations(name);



--------------------------------------------------------------------
-- ITEMS
--------------------------------------------------------------------
-- Búsquedas por código (lo principal)
CREATE INDEX IF NOT EXISTS idx_items_barcode ON items(barcode);

-- Filtrado por tipo
CREATE INDEX IF NOT EXISTS idx_items_type ON items(type_id);

-- Filtrado por estado
CREATE INDEX IF NOT EXISTS idx_items_state ON items(state_id);

-- Filtrado por ubicación física
CREATE INDEX IF NOT EXISTS idx_items_location ON items(location_id);

-- Consultas por dueño (polimórfico)
CREATE INDEX IF NOT EXISTS idx_items_owner ON items(owner_type_id, owner_id);

-- Listados frecuentes por nombre
CREATE INDEX IF NOT EXISTS idx_items_name ON items(name);



--------------------------------------------------------------------
-- ITEMS_CHECKOUT
--------------------------------------------------------------------
-- Consultas por usuario
CREATE INDEX IF NOT EXISTS idx_checkout_user ON items_checkout(user_id);

-- Consultas por ítem
CREATE INDEX IF NOT EXISTS idx_checkout_item ON items_checkout(item_id);

-- Fechas de checkout (historial ordenado)
CREATE INDEX IF NOT EXISTS idx_checkout_date ON items_checkout(checkout_at);



--------------------------------------------------------------------
-- ITEMS_CHECKOUT_ARCHIVE
--------------------------------------------------------------------
-- Consultas por usuario
CREATE INDEX IF NOT EXISTS idx_checkout_archive_user ON items_checkout_archive(user_id);

-- Consultas por item
CREATE INDEX IF NOT EXISTS idx_checkout_archive_item ON items_checkout_archive(item_id);

-- Archivo ordenado por fecha
CREATE INDEX IF NOT EXISTS idx_checkout_archive_date ON items_checkout_archive(archived_at);



--------------------------------------------------------------------
-- INSERTS
--------------------------------------------------------------------
INSERT INTO item_types (name) VALUES
    ('Aparato'),
    ('Desarrollo'),
    ('Equipo'),
    ('Herramienta'),
    ('Instrumento'),
    ('Insumo'),
    ('Máquina'),
    ('Mueble');

INSERT INTO item_states (name) VALUES
    ('Excelente'),
    ('Muy Bueno'),
    ('Bueno'),
    ('Regular'),
    ('Malo'),
    ('Muy Malo');

INSERT INTO owner_types (name) VALUES
    ('Persona'),
    ('Ubicación');

INSERT INTO locations (name) VALUES
    ('Sin definir');

INSERT INTO persons (first_name, last_name) VALUES
    ('Sin', 'definir');

INSERT INTO institutions (name) VALUES
    ('Sin definir');

