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
-- TOOLS TABLE
-- =============================================
CREATE TABLE IF NOT EXISTS tools (
    id INTEGER PRIMARY KEY,
    name VARCHAR(50) NOT NULL,
    barcode VARCHAR(50) NOT NULL UNIQUE
);

-- =============================================
-- TOOLS_CHECKOUT TABLE
-- =============================================
CREATE TABLE IF NOT EXISTS tools_checkout (
    id INTEGER PRIMARY KEY,
    original_id INTEGER,
    user_id INTEGER,
    tool_id INTEGER,
    checkout_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    returned_at TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
    FOREIGN KEY (tool_id) REFERENCES tools(id) ON DELETE CASCADE
);

-- =============================================
-- TOOLS_CHECKOUT_ARCHIVE TABLE
-- =============================================
CREATE TABLE IF NOT EXISTS tools_checkout_archive (
    id INTEGER PRIMARY KEY,
    user_id INTEGER NOT NULL,
    tool_id INTEGER NOT NULL,
    checkout_at TIMESTAMP NOT NULL,
    returned_at TIMESTAMP,
    archived_at TIMESTAMP NOT NULL,
    archive_reason TEXT
);
