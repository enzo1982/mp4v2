/^Repository UUID: ?/    { printf("%srepo_uuid=\"%s\"\n", prefix, $2) }
/^Last Changed Rev: ?/   { printf("%srepo_rev=\"%s\"\n", prefix, $2) }
/^Last Changed Date: ?/  { printf("%srepo_date=\"%s\"\n", prefix, $2) }

/^URL: ?/                { printf("%srepo_url=\"%s\"\n", prefix, $2); rtype="developer" }
/^URL: .+\/release\/.+/  { rtype="release" }
/^URL: .+\/snapshot\/.+/ { rtype="snapshot" }

END                      { printf("%srepo_type=\"%s\"\n", prefix, rtype) }
