# Pulls an image from docker hub and re-tags it for local development

$repo = "obroomhall/mediocre"
$tagToPull = Read-Host -Prompt "Which tag would you like to pull? [dev-master]"

if (!$tagToPull) {
    $tagToPull = "dev-master"
}

docker pull ${repo}:${tagToPull}

if ($?) {
    Write-Host "Retagging ${repo}:${tagToPull} as mediocre:dev-local"
    docker tag ${repo}:${tagToPull} mediocre:dev-local
}
